import shutil
import subprocess
import sys
import tarfile
import tempfile
import zipfile
from pathlib import Path

from util import REPO_ROOT, log


class UpdateError(Exception):
    pass


PLATFORM_PATTERNS = {
    "mac": "*mac*-installer.dmg",
    "ios": "*ios*-installer.dmg",
    "linux": "*linux*.tar.gz",
    "win": "*win*-installer.exe",
    "html5": "*html5*.zip",
    "android": "*android*.tar.gz",
}


_LOG_FILE = None


def _get_log_file():
    global _LOG_FILE
    if _LOG_FILE is None:
        _LOG_FILE = tempfile.NamedTemporaryFile(
            mode="w",
            prefix="fmod_update_",
            suffix=".log",
            delete=False,
        )
        log.debug("Command output: %s", _LOG_FILE.name)
    return _LOG_FILE


def _run(cmd: list[str], **kwargs) -> subprocess.CompletedProcess:
    f = _get_log_file()
    f.write(f"\n{'=' * 60}\n$ {' '.join(cmd)}\n{'=' * 60}\n")
    f.flush()
    kwargs.setdefault("stdout", f)
    kwargs.setdefault("stderr", subprocess.STDOUT)
    return subprocess.run(cmd, check=True, **kwargs)


def _copy(src: str | Path, dst: str | Path) -> None:
    dst = Path(dst)
    dst.parent.mkdir(parents=True, exist_ok=True)
    shutil.copy2(src, dst, follow_symlinks=True)


def _find_file(root: Path, pattern: str) -> Path:
    matches = list(root.rglob(pattern))
    if not matches:
        raise FileNotFoundError(f"No file matching '{pattern}' under {root}")
    return matches[0]


def _discover_archives(directory: Path) -> dict[str, Path]:
    archives = {}
    for platform, pattern in PLATFORM_PATTERNS.items():
        matches = sorted(directory.glob(pattern))
        if len(matches) == 1:
            archives[platform] = matches[0]
        elif len(matches) > 1:
            log.warning("Multiple %s archives found, using %s", platform, matches[0].name)
            for m in matches:
                log.warning("  %s", m.name)
            archives[platform] = matches[0]
    return archives


def _extract_tar(archive: Path, dest: Path, *, strip: int = 0) -> None:
    dest.mkdir(parents=True, exist_ok=True)
    with tarfile.open(archive) as tf:
        for member in tf.getmembers():
            parts = Path(member.name).parts
            if len(parts) <= strip:
                continue
            member.name = str(Path(*parts[strip:]))
            tf.extract(member, dest, filter="data")


def _extract_zip(archive: Path, dest: Path) -> None:
    dest.mkdir(parents=True, exist_ok=True)
    with zipfile.ZipFile(archive) as zf:
        for member in zf.namelist():
            target = (dest / member).resolve()
            if not target.is_relative_to(dest.resolve()):
                raise ValueError(f"Zip path traversal detected: {member}")
        zf.extractall(dest)


def _extract_exe(installer: Path, dest: Path) -> None:
    dest.mkdir(parents=True, exist_ok=True)
    tool = shutil.which("7z") or shutil.which("unar")
    if not tool:
        raise UpdateError(
            "Neither '7z' nor 'unar' found. One is required to extract Windows installers."
        )
    if "7z" in Path(tool).name:
        _run(["7z", "x", str(installer), f"-o{dest}"])
    else:
        _run(["unar", str(installer), "-o", str(dest)])


def _combine_ar_libs(dest_dir: Path, wasm_a: Path, bindings_a: Path, output_name: str) -> None:
    dest_dir.mkdir(parents=True, exist_ok=True)
    tmp_wasm = dest_dir / "tmp_wasm.a"
    tmp_bind = dest_dir / "tmp_bindings.a"
    shutil.copy2(wasm_a, tmp_wasm)
    shutil.copy2(bindings_a, tmp_bind)

    ar_script = (
        f"CREATE {dest_dir / output_name}\nADDLIB {tmp_wasm}\nADDLIB {tmp_bind}\nSAVE\nEND\n"
    )
    _run(["ar", "-M"], input=ar_script.encode(), cwd=str(dest_dir))
    tmp_wasm.unlink()
    tmp_bind.unlink()


def _codesign(path: Path) -> None:
    log.debug("Ad-hoc signing: %s", path.name)
    _run(["codesign", "--force", "--deep", "--sign", "-", str(path)])


def update_macos(archive: Path, tmpdir: Path) -> None:
    log.debug("Updating macOS...")
    mp = tmpdir / "mac"
    mp.mkdir()
    _run(["hdiutil", "attach", str(archive), "-mountpoint", str(mp)])
    try:
        api = mp / "FMOD Programmers API" / "api"
        dest = REPO_ROOT / "fmod" / "lib" / "osx"
        _copy(api / "core" / "lib" / "libfmod.dylib", dest / "libfmod.dylib")
        _copy(api / "studio" / "lib" / "libfmodstudio.dylib", dest / "libfmodstudio.dylib")
    finally:
        _run(["hdiutil", "detach", str(mp)])
    _codesign(dest / "libfmod.dylib")
    _codesign(dest / "libfmodstudio.dylib")


def update_ios(archive: Path, tmpdir: Path) -> None:
    log.debug("Updating iOS...")
    mp = tmpdir / "ios"
    mp.mkdir()
    _run(["hdiutil", "attach", str(archive), "-mountpoint", str(mp)])
    try:
        api = mp / "FMOD Programmers API" / "api"
        arm_dest = REPO_ROOT / "fmod" / "lib" / "arm64-ios"
        sim_dest = REPO_ROOT / "fmod" / "lib" / "x86_64-ios"
        _copy(api / "core" / "lib" / "libfmod_iphoneos.a", arm_dest / "libfmod.a")
        _copy(api / "studio" / "lib" / "libfmodstudio_iphoneos.a", arm_dest / "libfmodstudio.a")
        _copy(api / "core" / "lib" / "libfmod_iphonesimulator.a", sim_dest / "libfmod.a")
        _copy(
            api / "studio" / "lib" / "libfmodstudio_iphonesimulator.a", sim_dest / "libfmodstudio.a"
        )
    finally:
        _run(["hdiutil", "detach", str(mp)])
    _codesign(arm_dest / "libfmod.a")
    _codesign(arm_dest / "libfmodstudio.a")
    _codesign(sim_dest / "libfmod.a")
    _codesign(sim_dest / "libfmodstudio.a")


def _require_patchelf() -> str:
    patchelf = shutil.which("patchelf")
    if not patchelf:
        raise UpdateError(
            "'patchelf' not found. It is required to fix Linux SONAME versioning.\n"
            "Install it with: sudo apt install patchelf  (or equivalent)"
        )
    return patchelf


def _patch_linux_sonames(dest: Path) -> None:
    # Workaround (soon deprecated): FMOD ships Linux .so files with versioned
    # SONAMEs (e.g. libfmod.so.14) and NEEDED refs (libfmodstudio.so needs
    # libfmod.so.14). Defold's dynamicLibs only copies the unversioned
    # filename, so the linker fails at runtime. We patch both SONAME and
    # NEEDED entries until Defold supports versioned .so references.
    # Fixed upstream: https://github.com/defold/defold/issues/11558
    # Merged PR:      https://github.com/defold/defold/pull/11626
    patchelf = _require_patchelf()
    for lib_name in ("libfmod.so", "libfmodstudio.so"):
        lib = dest / lib_name
        log.debug("Patching ELF: %s", lib_name)
        _run([patchelf, "--set-soname", lib_name, str(lib)])
    # libfmodstudio.so has NEEDED: libfmod.so.XX -> libfmod.so
    studio = dest / "libfmodstudio.so"
    result = subprocess.run(
        [patchelf, "--print-needed", str(studio)],
        capture_output=True,
        text=True,
        check=True,
    )
    for needed in result.stdout.splitlines():
        if needed.startswith("libfmod.so.") and needed != "libfmod.so":
            log.debug("Patching NEEDED: %s -> libfmod.so", needed)
            _run([patchelf, "--replace-needed", needed, "libfmod.so", str(studio)])


def update_linux(archive: Path, tmpdir: Path) -> None:
    log.debug("Updating Linux...")
    d = tmpdir / "linux"
    _extract_tar(archive, d, strip=1)

    dest_x86_64 = REPO_ROOT / "fmod" / "lib" / "x86_64-linux"
    _copy(d / "api" / "core" / "lib" / "x86_64" / "libfmod.so", dest_x86_64 / "libfmod.so")
    _copy(
        d / "api" / "studio" / "lib" / "x86_64" / "libfmodstudio.so",
        dest_x86_64 / "libfmodstudio.so",
    )
    _patch_linux_sonames(dest_x86_64)

    dest_arm64 = REPO_ROOT / "fmod" / "lib" / "arm64-linux"
    _copy(d / "api" / "core" / "lib" / "arm64" / "libfmod.so", dest_arm64 / "libfmod.so")
    _copy(
        d / "api" / "studio" / "lib" / "arm64" / "libfmodstudio.so", dest_arm64 / "libfmodstudio.so"
    )
    _patch_linux_sonames(dest_arm64)


def update_windows(archive: Path, tmpdir: Path) -> None:
    log.debug("Updating Windows...")
    d = tmpdir / "win"
    _extract_exe(archive, d)

    dest64 = REPO_ROOT / "fmod" / "lib" / "x86_64-win32"
    _copy(_find_file(d, "api/core/lib/x64/fmod.dll"), dest64 / "fmod.dll")
    _copy(_find_file(d, "api/core/lib/x64/fmod_vc.lib"), dest64 / "fmod.lib")
    _copy(_find_file(d, "api/studio/lib/x64/fmodstudio.dll"), dest64 / "fmodstudio.dll")
    _copy(_find_file(d, "api/studio/lib/x64/fmodstudio_vc.lib"), dest64 / "fmodstudio.lib")

    dest32 = REPO_ROOT / "fmod" / "lib" / "x86-win32"
    _copy(_find_file(d, "api/core/lib/x86/fmod.dll"), dest32 / "fmod.dll")
    _copy(_find_file(d, "api/core/lib/x86/fmod_vc.lib"), dest32 / "fmod.lib")
    _copy(_find_file(d, "api/studio/lib/x86/fmodstudio.dll"), dest32 / "fmodstudio.dll")
    _copy(_find_file(d, "api/studio/lib/x86/fmodstudio_vc.lib"), dest32 / "fmodstudio.lib")


def update_html5(archive: Path, tmpdir: Path) -> None:
    log.debug("Updating HTML5...")
    d = tmpdir / "html5"
    _extract_zip(archive, d)

    log.debug("Combining regular WASM libraries...")
    wasm_dest = REPO_ROOT / "fmod" / "lib" / "wasm-web"
    _combine_ar_libs(
        wasm_dest,
        _find_file(d, "api/studio/lib/w32/fmodstudio_wasm.a"),
        _find_file(d, "api/studio/lib/w32/fmodstudio_bindings.a"),
        "libfmodstudio.a",
    )

    log.debug("Combining pthread WASM libraries...")
    pthread_dest = REPO_ROOT / "fmod" / "lib" / "wasm_pthread-web"
    _combine_ar_libs(
        pthread_dest,
        _find_file(d, "api/studio/lib/w32/fmodstudioP_wasm.a"),
        _find_file(d, "api/studio/lib/w32/fmodstudioP_bindings.a"),
        "libfmodstudio.a",
    )


def update_android(archive: Path, tmpdir: Path) -> None:
    log.debug("Updating Android...")
    d = tmpdir / "android"
    _extract_tar(archive, d, strip=1)

    arm64 = REPO_ROOT / "fmod" / "lib" / "arm64-android"
    _copy(d / "api" / "core" / "lib" / "arm64-v8a" / "libfmod.so", arm64 / "libfmod.so")
    _copy(
        d / "api" / "studio" / "lib" / "arm64-v8a" / "libfmodstudio.so", arm64 / "libfmodstudio.so"
    )

    armv7 = REPO_ROOT / "fmod" / "lib" / "armv7-android"
    _copy(d / "api" / "core" / "lib" / "armeabi-v7a" / "libfmod.so", armv7 / "libfmod.so")
    _copy(
        d / "api" / "studio" / "lib" / "armeabi-v7a" / "libfmodstudio.so",
        armv7 / "libfmodstudio.so",
    )

    jar_dest = REPO_ROOT / "fmod" / "lib" / "android"
    _copy(d / "api" / "core" / "lib" / "fmod.jar", jar_dest / "fmod.jar")


def update_headers(tmpdir: Path) -> None:
    log.debug("Copying FMOD headers...")
    linux_dir = tmpdir / "linux"
    dest = REPO_ROOT / "fmod" / "include"

    for old in dest.glob("fmod*"):
        old.unlink()

    for src in (linux_dir / "api" / "core" / "inc").iterdir():
        if src.suffix in (".h", ".hpp"):
            _copy(src, dest / src.name)
    for src in (linux_dir / "api" / "studio" / "inc").iterdir():
        if src.suffix in (".h", ".hpp"):
            _copy(src, dest / src.name)


PLATFORM_HANDLERS = {
    "mac": update_macos,
    "ios": update_ios,
    "linux": update_linux,
    "win": update_windows,
    "html5": update_html5,
    "android": update_android,
}

APPLE_PLATFORMS = {"mac", "ios"}


def run_update(directory: Path, ignore_apple: bool) -> None:
    directory = directory.resolve()
    if not directory.is_dir():
        raise UpdateError(f"Not a directory: {directory}")

    archives = _discover_archives(directory)

    if not archives:
        patterns = "\n".join(f"  {p}: {pat}" for p, pat in PLATFORM_PATTERNS.items())
        raise UpdateError(
            f"No FMOD distribution files found in {directory}\nExpected files matching:\n{patterns}"
        )

    log.debug("Discovered archives:")
    for platform, path in sorted(archives.items()):
        log.debug("  %s: %s", platform, path.name)

    if not ignore_apple and APPLE_PLATFORMS & archives.keys():
        if not shutil.which("hdiutil"):
            raise UpdateError(
                "'hdiutil' not found. macOS/iOS updates require macOS.\n"
                "Re-run with --ignore-apple / -ia to skip them:\n"
                f"  python {sys.argv[0]} update --ignore-apple {directory}"
            )

    skip = APPLE_PLATFORMS if ignore_apple else set()

    try:
        with tempfile.TemporaryDirectory() as tmpdir_str:
            tmpdir = Path(tmpdir_str)

            for platform, handler in PLATFORM_HANDLERS.items():
                if platform in skip:
                    continue
                if platform not in archives:
                    log.debug("Skipping %s (no archive found)", platform)
                    continue
                try:
                    handler(archives[platform], tmpdir)
                except subprocess.CalledProcessError as e:
                    raise UpdateError(f"{platform} failed: {e}") from e
                except (OSError, FileNotFoundError) as e:
                    raise UpdateError(f"{platform} failed: {e}") from e
                log.info("Updated %s", platform)

            # update_linux must precede update_headers (headers come from the linux archive)
            if "linux" in archives:
                update_headers(tmpdir)

        log.info("Done!")
    finally:
        if _LOG_FILE is not None:
            log.info("Command output log: %s", _LOG_FILE.name)
            _LOG_FILE.close()
