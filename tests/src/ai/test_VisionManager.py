import sys
import os
import pytest

sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), "../../..")))

from ai.src.VisionManager import VisionManager


def test_vision_manager_initialization():
    vm = VisionManager()
    assert vm.tiles == []
    assert vm.current_level == 1


def test_set_level_and_update_tiles():
    vm = VisionManager()

    vm.set_level(3)
    assert vm.current_level == 3

    mock_tiles = [["player"], ["food"], ["linemate"]]
    vm.update_tiles(mock_tiles)
    assert vm.tiles == mock_tiles


def test_resource_management_underfoot():
    vm = VisionManager()
    vm.update_tiles([["player", "food"], ["linemate"]])

    vm.remove_resource_underfoot("food")
    assert "food" not in vm.tiles[0]
    assert "player" in vm.tiles[0]

    vm.add_resource_underfoot("sibur")
    assert "sibur" in vm.tiles[0]


def test_get_tile_index_of():
    vm = VisionManager()
    vm.update_tiles([["player"], ["food"], [], ["linemate"]])
    assert vm.get_tile_index_of("food") == 1
    assert vm.get_tile_index_of("linemate") == 3
    assert vm.get_tile_index_of("thystame") is None


def test_reset_on_turn():
    vm = VisionManager()
    vm.update_tiles([["player"], ["food"]])
    vm.reset_on_turn()
    assert vm.tiles == []


def test_shift_on_forward_level_1():
    vm = VisionManager()
    vm.set_level(1)
    initial_tiles = [["player"], ["food"], ["linemate"], ["deraumere"]]
    vm.update_tiles(initial_tiles)
    vm.shift_on_forward()

    assert len(vm.tiles) == 4
    assert vm.tiles[0] == ["linemate"]
    assert vm.tiles[1] == []
    assert vm.tiles[2] == []
    assert vm.tiles[3] == []


def test_shift_on_forward_empty():
    vm = VisionManager()
    vm.tiles = []
    vm.shift_on_forward()
    assert vm.tiles == []
