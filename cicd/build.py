#!/usr/bin/env python3
import argparse
import logging
import shutil
import subprocess
import sys
import unittest
from pathlib import Path

from util import REPO_ROOT, SCRIPT_DIR, log

_FMOD_SRC = REPO_ROOT / "fmod" / "src"
_FMOD_INCLUDE = REPO_ROOT / "fmod" / "include"
_GENERATED = {_FMOD_SRC / "fmod_generated.c"}


def _c_source_files() -> list[Path]:
    exts = (".c", ".cpp", ".h", ".hpp", ".m")
    return [f for f in sorted(_FMOD_SRC.iterdir()) if f.suffix in exts and f not in _GENERATED]


def _lua_files() -> list[Path]:
    exts = ("*.script", "*.gui_script", "*.render_script", "*.lua")
    files: list[Path] = []
    for ext in exts:
        files.extend((REPO_ROOT / "example").rglob(ext))
    return sorted(files)


def update(args: argparse.Namespace) -> None:
    from update import UpdateError, run_update

    try:
        run_update(Path(args.directory), args.ignore_apple)
    except UpdateError as e:
        log.error("%s", e)
        sys.exit(1)


def generate(_: argparse.Namespace) -> None:
    log.debug("Generating bindings...")
    from generate_bindings import write_generated_bindings

    write_generated_bindings()
    log.info("Bindings generated.")


def lint(_: argparse.Namespace) -> None:
    log.debug("Running ruff lint...")
    result = subprocess.run(["ruff", "check", str(SCRIPT_DIR)])
    if result.returncode != 0:
        sys.exit(1)
    log.info("Ruff lint passed.")

    if shutil.which("luacheck"):
        lua_files = _lua_files()
        if lua_files:
            log.debug("Running luacheck...")
            result = subprocess.run(["luacheck"] + [str(f) for f in lua_files])
            if result.returncode != 0:
                sys.exit(1)
            log.info("Luacheck passed.")
        else:
            log.debug("No Lua files found to check.")
    else:
        log.debug("luacheck not found, skipping Lua lint.")

    if shutil.which("cppcheck"):
        log.debug("Running cppcheck...")
        cppcheck_files = [f for f in _c_source_files() if f.suffix != ".m"]
        result = subprocess.run(
            [
                "cppcheck",
                "--enable=warning,performance,portability",
                "--error-exitcode=1",
                "--suppress=missingIncludeSystem",
                "--suppress=toomanyconfigs",
                f"-I{_FMOD_INCLUDE}",
                f"-I{_FMOD_SRC}",
            ]
            + [str(f) for f in cppcheck_files]
        )
        if result.returncode != 0:
            sys.exit(1)
        log.info("cppcheck passed.")
    else:
        log.debug("cppcheck not found, skipping C/C++ static analysis.")


def format_check(_: argparse.Namespace) -> None:
    log.debug("Running ruff format check...")
    result = subprocess.run(["ruff", "format", "--check", str(SCRIPT_DIR)])
    if result.returncode != 0:
        log.error("ruff format check failed. Run 'python cicd/build.py format' to fix.")
        sys.exit(1)
    log.info("ruff format check passed.")

    if shutil.which("stylua"):
        lua_files = _lua_files()
        if lua_files:
            log.debug("Running stylua check...")
            result = subprocess.run(["stylua", "--check"] + [str(f) for f in lua_files])
            if result.returncode != 0:
                log.error("stylua check failed. Run 'python cicd/build.py format' to fix.")
                sys.exit(1)
            log.info("stylua check passed.")
        else:
            log.debug("No Lua files found to check.")
    else:
        log.debug("stylua not found, skipping Lua format check.")

    if shutil.which("clang-format"):
        log.debug("Running clang-format check...")
        c_files = _c_source_files()
        result = subprocess.run(
            ["clang-format", "--dry-run", "--Werror"] + [str(f) for f in c_files]
        )
        if result.returncode != 0:
            log.error("clang-format check failed. Run 'python cicd/build.py format' to fix.")
            sys.exit(1)
        log.info("clang-format check passed.")
    else:
        log.debug("clang-format not found, skipping C/C++ format check.")


def format_code(_: argparse.Namespace) -> None:
    log.debug("Running ruff format...")
    result = subprocess.run(["ruff", "format", str(SCRIPT_DIR)])
    if result.returncode != 0:
        sys.exit(1)
    log.info("Ruff format done.")

    if shutil.which("stylua"):
        lua_files = _lua_files()
        if lua_files:
            log.debug("Running stylua...")
            result = subprocess.run(["stylua"] + [str(f) for f in lua_files])
            if result.returncode != 0:
                sys.exit(1)
            log.info("stylua done.")
        else:
            log.debug("No Lua files found to format.")
    else:
        log.debug("stylua not found, skipping Lua formatting.")

    if shutil.which("clang-format"):
        log.debug("Running clang-format...")
        c_files = _c_source_files()
        result = subprocess.run(["clang-format", "-i"] + [str(f) for f in c_files])
        if result.returncode != 0:
            sys.exit(1)
        log.info("clang-format done.")
    else:
        log.debug("clang-format not found, skipping C/C++ formatting.")


DEPENDENCIES = {
    "generate": {
        "gcc": {"cmd": "gcc", "hint": "apt install gcc"},
        "pycparser": {
            "module": "pycparser",
            "hint": "pip install -r cicd/requirements.txt",
        },
        "jinja2": {"module": "jinja2", "hint": "pip install -r cicd/requirements.txt"},
    },
    "update": {
        "7z or unar": {
            "cmd": ["7z", "unar"],
            "hint": "apt install p7zip-full  OR  apt install unar",
        },
        "ar": {"cmd": "ar", "hint": "apt install binutils"},
        "patchelf": {
            "cmd": "patchelf",
            "hint": "apt install patchelf",
            "platform": "linux",
        },
        "codesign": {
            "cmd": "codesign",
            "hint": "macOS only (ad-hoc signs extracted libs)",
            "platform": "darwin",
        },
        "hdiutil": {"cmd": "hdiutil", "hint": "macOS only", "platform": "darwin"},
    },
    "lint/format": {
        "ruff": {"cmd": "ruff", "hint": "pip install -r cicd/requirements.txt"},
        "luacheck": {"cmd": "luacheck", "hint": "luarocks install luacheck"},
        "stylua": {"cmd": "stylua", "hint": "cargo install stylua"},
        "clang-format": {"cmd": "clang-format", "hint": "apt install clang-format"},
        "cppcheck": {"cmd": "cppcheck", "hint": "apt install cppcheck"},
    },
}


def _check_dep(_: str, dep: dict) -> bool:
    if "module" in dep:
        try:
            __import__(dep["module"])
            return True
        except ImportError:
            return False
    cmds = dep["cmd"] if isinstance(dep["cmd"], list) else [dep["cmd"]]
    return any(shutil.which(c) for c in cmds)


def health(_: argparse.Namespace) -> None:
    ok = True
    for group, deps in DEPENDENCIES.items():
        log.debug("[%s]", group)
        for name, dep in deps.items():
            platform = dep.get("platform")
            if platform and not sys.platform.startswith(platform):
                log.debug("  %-16s skipped (not %s)", name, platform)
                continue
            found = _check_dep(name, dep)
            if found:
                log.info("  %-16s found", name)
            else:
                log.error("  %-16s MISSING  (%s)", name, dep["hint"])
                ok = False
    if not ok:
        sys.exit(1)
    log.info("All dependencies found.")


def test(_: argparse.Namespace) -> None:
    log.debug("Running tests...")
    import test_api_from_bindings

    loader = unittest.TestLoader()
    suite = loader.loadTestsFromModule(test_api_from_bindings)
    runner = unittest.TextTestRunner(verbosity=2)
    result = runner.run(suite)
    if not result.wasSuccessful():
        sys.exit(1)
    log.info("All tests passed.")


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        prog="build.py",
        description=(
            "CI/CD tool for extension-fmod\n\n"
            "Manages FMOD library updates from platform archives, C/Lua "
            "binding generation, and test execution."
        ),
        formatter_class=argparse.RawDescriptionHelpFormatter,
    )
    parser.add_argument(
        "--verbose",
        "-v",
        action="store_true",
        help="Enable debug logging output.",
    )

    subparsers = parser.add_subparsers(
        title="commands",
        dest="command",
        description="Available sub-commands.",
    )

    update_parser = subparsers.add_parser(
        "update",
        help="Extract FMOD libraries from platform distribution archives.",
        description=(
            "Scan a directory for FMOD distribution archives and copies them by "
            "filename pattern (e.g. *linux*.tar.gz, *win*-installer.exe). Platforms "
            "without a matching archive are skipped."
        ),
    )
    update_parser.add_argument(
        "directory",
        help="Directory containing the FMOD distribution archives.",
    )
    update_parser.add_argument(
        "--ignore-apple",
        "-ia",
        action="store_true",
        help="Skip macOS and iOS updates (which require hdiutil on macOS).",
    )
    update_parser.set_defaults(func=update)

    generate_parser = subparsers.add_parser(
        "generate",
        help="Generate C and Lua bindings from FMOD headers.",
        description=("Parses FMOD C headers, generate C, Lua bindings and fmod.script_api.\n"),
    )
    generate_parser.set_defaults(func=generate)

    lint_parser = subparsers.add_parser(
        "lint",
        help="Run linters (ruff, luacheck, cppcheck).",
    )
    lint_parser.set_defaults(func=lint)

    format_check_parser = subparsers.add_parser(
        "format-check",
        help="Check formatting (ruff, stylua, clang-format).",
    )
    format_check_parser.set_defaults(func=format_check)

    format_parser = subparsers.add_parser(
        "format",
        help="Format code (ruff for Python, stylua for Lua, clang-format for C/C++).",
    )
    format_parser.set_defaults(func=format_code)

    health_parser = subparsers.add_parser(
        "health",
        help="Verify all external dependencies are available.",
    )
    health_parser.set_defaults(func=health)

    test_parser = subparsers.add_parser(
        "test",
        help="Run the Python test suite for code generation utilities.",
        description=("Run Unittest Test Suites"),
    )
    test_parser.set_defaults(func=test)

    args = parser.parse_args()
    if not hasattr(args, "func"):
        parser.print_help()
        sys.exit(1)

    return args


def main() -> None:
    args = parse_args()
    if args.verbose:
        log.setLevel(logging.DEBUG)
    args.func(args)


if __name__ == "__main__":
    main()
