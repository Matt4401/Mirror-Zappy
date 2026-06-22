import sys
import os
import pytest
from unittest.mock import patch, MagicMock

sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), "../../..")))

from ai.zappy_ai import main


@patch("src.AITeamClass.Connection")
def test_main(mock_connect_class):
    mock_instance = MagicMock()
    mock_connect_class.return_value = mock_instance

    test_args = ["zappy_ai", "-p", "4242", "-n", "EquipeTest", "-h", "localhost"]
    with patch("sys.argv", test_args):
        main()


@patch("src.AITeamClass.Connection")
def test_invalid_main_zero_args(mock_connect):
    mock_connect.return_value = None
    test_wrong_args = ["zappy_ai"]

    with patch("sys.argv", test_wrong_args):
        with pytest.raises(SystemExit) as excinfo:
            main()
        assert excinfo.value.code == 84


@patch("src.AITeamClass.Connection")
def test_invalid_main_zero_args(mock_connect):
    mock_connect.return_value = None
    test_wrong_args = ["zappy_ai", "-p", "4242", "-n", "EquipeTest", "-h"]

    with patch("sys.argv", test_wrong_args):
        with pytest.raises(SystemExit) as excinfo:
            main()
        assert excinfo.value.code == 84
