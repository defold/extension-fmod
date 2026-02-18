import logging
import sys
from pathlib import Path

SCRIPT_DIR = Path(__file__).resolve().parent
REPO_ROOT = SCRIPT_DIR.parent

COLORS = {
    logging.DEBUG: "",  # default (progress)
    logging.INFO: "\033[32m",  # green (success)
    logging.WARNING: "\033[33m",  # yellow
    logging.ERROR: "\033[31m",  # red
    logging.CRITICAL: "\033[1;31m",  # bold red
}
RESET = "\033[0m"


class ColorFormatter(logging.Formatter):
    def format(self, record):
        msg = super().format(record)
        if sys.stderr.isatty():
            color = COLORS.get(record.levelno, "")
            return f"{color}{msg}{RESET}"
        return msg


def get_logger(name: str = "cicd") -> logging.Logger:
    logger = logging.getLogger(name)
    if not logger.handlers:
        handler = logging.StreamHandler()
        handler.setFormatter(ColorFormatter("%(levelname)s %(message)s"))
        logger.addHandler(handler)
        logger.setLevel(logging.DEBUG)
    return logger


log = get_logger()
