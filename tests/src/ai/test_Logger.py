import logging
import os
import sys
from pathlib import Path
from unittest.mock import MagicMock, patch
import pytest

from src.util.InitLogger import PlayerLogger, ColorFormatter, TagFilter


@pytest.fixture(autouse=True)
def cleanup_logging():
    yield
    logger = logging.getLogger("player_42")
    for handler in logger.handlers[:]:
        logger.removeHandler(handler)


@patch("src.util.InitLogger.Path.mkdir")
@patch("src.util.InitLogger.logging.FileHandler")
@patch("src.util.InitLogger.logging.StreamHandler")
def test_setup_logging_success(mock_stream_handler, mock_file_handler, mock_mkdir):
    mock_console_instance = MagicMock()
    mock_file_instance = MagicMock()

    mock_stream_handler.return_value = mock_console_instance
    mock_file_handler.return_value = mock_file_instance

    test_player_id = "42"

    logger = PlayerLogger.setup_logging(test_player_id, fresh=True)

    mock_mkdir.assert_called_once_with(parents=True, exist_ok=True)

    assert logger.name == "player_42"
    assert logger.level == logging.INFO
    assert logger.propagate is False

    mock_stream_handler.assert_called_once_with(sys.stdout)
    mock_console_instance.setLevel.assert_called_with(logging.INFO)
    mock_console_instance.setFormatter.assert_called_once()
    mock_console_instance.addFilter.assert_called_once()

    root_project = Path(__file__).resolve().parents[3]
    expected_log_file = root_project / "logs" / "ai.log"

    mock_file_handler.assert_called_once_with(
        expected_log_file, mode="w", encoding="utf-8"
    )
    mock_file_instance.setLevel.assert_called_with(logging.INFO)
    mock_file_instance.setFormatter.assert_called_once()
    mock_file_instance.addFilter.assert_called_once()

    assert mock_console_instance in logger.handlers
    assert mock_file_instance in logger.handlers



def test_tag_filter():
    filter_tag = "p1234"
    tag_filter = TagFilter(filter_tag)

    record = logging.LogRecord("name", logging.INFO, "path", 1, "msg", None, None)

    assert not hasattr(record, "tag")
    result = tag_filter.filter(record)

    assert result is True
    assert record.tag == filter_tag


def test_color_formatter_with_color():
    formatter = ColorFormatter(fmt="%(message)s", datefmt=None, use_color=True)
    record = logging.LogRecord("name", logging.ERROR, "path", 1, "Test Error", None, None)

    formatted_text = formatter.format(record)
    assert formatted_text.startswith("\033[31m")
    assert formatted_text.endswith("\033[0m")


def test_color_formatter_without_color():
    formatter = ColorFormatter(fmt="%(message)s", datefmt=None, use_color=False)
    record = logging.LogRecord("name", logging.ERROR, "path", 1, "Test Error", None, None)

    formatted_text = formatter.format(record)
    assert formatted_text == "Test Error"