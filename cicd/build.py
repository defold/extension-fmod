#!/usr/bin/env python3
import argparse
import shutil
import subprocess
import sys
import unittest
from pathlib import Path

from util import SCRIPT_DIR, log


def update(args: argparse.Namespace) -> None:
    from update import run_update

    run_update(Path(args.directory), args.ignore_apple)


def generate(args: argparse.Namespace) -> None:
    log.debug("Generating bindings...")
    from generate_bindings import write_generated_bindings

    write_generated_bindings()
    log.info("Bindings generated.")


def lint(args: argparse.Namespace) -> None:
    log.debug("Running ruff lint...")
    result = subprocess.run(["ruff", "check", str(SCRIPT_DIR)])
    if result.returncode != 0:
        sys.exit(1)
    log.info("Lint passed.")


def format_code(args: argparse.Namespace) -> None:
    log.debug("Running ruff format...")
    subprocess.run(["ruff", "format", str(SCRIPT_DIR)])
    log.info("Format done.")


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
    },
}


def _check_dep(name: str, dep: dict) -> bool:
    if "module" in dep:
        try:
            __import__(dep["module"])
            return True
        except ImportError:
            return False
    cmds = dep["cmd"] if isinstance(dep["cmd"], list) else [dep["cmd"]]
    return any(shutil.which(c) for c in cmds)


def health(args: argparse.Namespace) -> None:
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


def test(args: argparse.Namespace) -> None:
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

    subparsers = parser.add_subparsers(
        title="commands",
        dest="command",
        description="Available sub-commands.",
    )

    update_parser = subparsers.add_parser(
        "update",
        help="Extract FMOD libraries from platform distribution archives.",
        description=(
            "Scan a directory for FMOD distribution archives and copies it by "
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
        description=(
            "Parses FMOD C headers, generate C, Lua bindings and fmod.script_api.\n"
        ),
    )
    generate_parser.set_defaults(func=generate)

    lint_parser = subparsers.add_parser(
        "lint",
        help="Run ruff linter on cicd/ Python files.",
    )
    lint_parser.set_defaults(func=lint)

    format_parser = subparsers.add_parser(
        "format",
        help="Format cicd/ Python files with ruff.",
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
    args.func(args)


if __name__ == "__main__":
    main()
