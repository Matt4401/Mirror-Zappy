import sys
import os
import pytest

sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), "../../..")))

from ai.src.util.InventoryClass import Inventory
from ai.src.ParseCommand import ParseCommand


def test_inventory():
    inventory = Inventory()
    parser = ParseCommand(inventory)
    cmd = (
        "linemate 5, deraumere 4, sibur 5, mendiane 0, phiras 55, thystame 8, food 444"
    )

    parser.parse_inventory(cmd)
    assert inventory.linemate == 5
    assert inventory.food == 444


def test_parse_inventory__missing_value():
    inventory = Inventory()
    parser = ParseCommand(inventory)
    cmd = "linemate 5 4, sibur 5, mendiane 0, phiras 55, thystame 8, food 444"

    with pytest.raises(ValueError) as e:
        parser.parse_inventory(cmd)
    assert "deraumere" in str(e.value)
