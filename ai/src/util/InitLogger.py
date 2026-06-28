import logging
import os
import sys
from pathlib import Path

LOG_FILENAME = "ai.log"
LINE_FORMAT = "%(asctime)s %(levelname)-7s [%(tag)s] %(message)s"
TIME_FORMAT = "%H:%M:%S"

_RESET = "\033[0m"
_LEVEL_COLORS = {
    "DEBUG": "\033[90m",
    "INFO": "\033[32m",
    "WARNING": "\033[33m",
    "ERROR": "\033[31m",
    "CRITICAL": "\033[97;41m",
}


class ColorFormatter(logging.Formatter):
    def __init__(self, fmt, datefmt, use_color):
        super().__init__(fmt, datefmt)
        self.use_color = use_color

    def format(self, record):
        text = super().format(record)
        if not self.use_color:
            return text
        color = _LEVEL_COLORS.get(record.levelname, "")
        return f"{color}{text}{_RESET}" if color else text


class TagFilter(logging.Filter):
    def __init__(self, tag):
        super().__init__()
        self.tag = tag

    def filter(self, record):
        record.tag = self.tag
        return True


class PlayerLogger:
    @staticmethod
    def setup_logging(player_id: str, fresh: bool = False):
        root_project = Path(__file__).resolve().parents[3]
        folder_logs = root_project / "logs"
        folder_logs.mkdir(parents=True, exist_ok=True)
        log_file = folder_logs / LOG_FILENAME

        tag = f"p{os.getpid()}"

        logger = logging.getLogger(f"player_{player_id}")
        logger.setLevel(logging.INFO)
        logger.propagate = False
        if logger.handlers:
            return logger

        tag_filter = TagFilter(tag)

        console = logging.StreamHandler(sys.stdout)
        console.setLevel(logging.INFO)
        console.setFormatter(
            ColorFormatter(LINE_FORMAT, TIME_FORMAT, use_color=sys.stdout.isatty())
        )
        console.addFilter(tag_filter)

        file_handler = logging.FileHandler(
            log_file, mode="w" if fresh else "a", encoding="utf-8"
        )
        file_handler.setLevel(logging.INFO)
        file_handler.setFormatter(logging.Formatter(LINE_FORMAT, TIME_FORMAT))
        file_handler.addFilter(tag_filter)

        logger.addHandler(console)
        logger.addHandler(file_handler)
        return logger
