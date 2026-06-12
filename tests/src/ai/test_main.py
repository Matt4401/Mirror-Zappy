import sys
import os
import pytest
from unittest.mock import patch

sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '../../..')))

from ai.zappy_ai import main

from mainAI import mainAI


def test_main():
    test_args = ["zappy_ai", "-p", "4242", "-n", "EquipeTest", "-h", "localhost"]
    with patch("sys.argv", test_args):
        main()


def test_invalide_main_zero_args():
    test_wrong_args = ["zappy_ai"]

    with patch("sys.argv", test_wrong_args):
        with pytest.raises(SystemExit) as excinfo:
            main()
        assert excinfo.value.code == 84


def test_invalide_main_zero_args():
    test_wrong_args = ["zappy_ai", "-p", "4242", "-n", "EquipeTest", "-h"]

    with patch("sys.argv", test_wrong_args):
        with pytest.raises(SystemExit) as excinfo:
            main()
        assert excinfo.value.code == 84
