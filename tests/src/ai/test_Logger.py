import sys
from unittest.mock import patch, mock_open, MagicMock
from pathlib import Path
import pytest

from src.util.InitLogger import PlayerLogger

FAKE_YAML_CONTENT = """
version: 1
handlers:
  console:
    class: logging.StreamHandler
    level: DEBUG
  network_file:
    filename: ~
  command_file:
    filename: ~
  player_file_template:
    level: INFO
    formatter: detailed
    filename: ~
    maxBytes: 100000
    backupCount: 5
loggers:
  player_template:
    level: INFO
    handlers: [console]
"""


@patch("yaml.safe_load")
@patch(
    "src.util.InitLogger.Path.open", new_callable=mock_open, read_data=FAKE_YAML_CONTENT
)
@patch("src.util.InitLogger.logging.config.dictConfig")
@patch("src.util.InitLogger.Path.mkdir")
def test_setup_logging(mock_mkdir, mock_dict_config, mock_open_file, mock_yaml_load):
    import yaml

    config_dict = yaml.unsafe_load(FAKE_YAML_CONTENT)
    mock_yaml_load.return_value = config_dict
    test_player_id = "42"

    PlayerLogger.setup_logging(test_player_id)

    assert mock_mkdir.call_count == 2
    mock_mkdir.assert_called_with(parents=True, exist_ok=True)

    mock_dict_config.assert_called_once()
    final_config = mock_dict_config.call_args[0][0]
    assert "player_file_template" not in final_config["handlers"]

    expected_handler_name = f"player_{test_player_id}_file"
    expected_logger_name = f"player_{test_player_id}"

    assert expected_handler_name in final_config["handlers"]
    assert expected_logger_name in final_config["loggers"]

    dynamic_handler = final_config["handlers"][expected_handler_name]
    assert dynamic_handler["class"] == "logging.handlers.RotatingFileHandler"
    assert f"player/player_{test_player_id}.log" in dynamic_handler["filename"]

    assert "server.log" in final_config["handlers"]["network_file"]["filename"]
    assert "commands.log" in final_config["handlers"]["command_file"]["filename"]
    assert (
        "player/server.log" not in final_config["handlers"]["network_file"]["filename"]
    )
