import sys
import os
import pytest
from unittest.mock import patch

sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), "../../..")))

from ai.src.fsm.TickManager import TickManager


def test_tick_manager_initialization():
    tm = TickManager(agent_id=3)
    assert tm.tick == 0
    assert tm.broadcast_offset == 21


@patch("ai.src.fsm.TickManager.INVENTORY_INTERVAL", 5)
@patch("ai.src.fsm.TickManager.LOOK_INTERVAL", 10)
@patch("ai.src.fsm.TickManager.BROADCAST_INTERVAL", 20)
def test_tick_update_no_commands():
    tm = TickManager(agent_id=0)
    cmds = tm.tick_update()
    assert tm.tick == 1
    assert cmds == []


@patch("ai.src.fsm.TickManager.INVENTORY_INTERVAL", 3)
@patch("ai.src.fsm.TickManager.LOOK_INTERVAL", 10)
@patch("ai.src.fsm.TickManager.BROADCAST_INTERVAL", 20)
def test_tick_should_inventory():
    tm = TickManager(agent_id=0)
    tm.tick_update()
    tm.tick_update()
    cmds = tm.tick_update()
    assert "Inventory" in cmds
    assert "Look" not in cmds


@patch("ai.src.fsm.TickManager.INVENTORY_INTERVAL", 10)
@patch("ai.src.fsm.TickManager.LOOK_INTERVAL", 4)
@patch("ai.src.fsm.TickManager.BROADCAST_INTERVAL", 20)
def test_tick_should_look():
    tm = TickManager(agent_id=0)
    tm.tick_update()
    tm.tick_update()
    tm.tick_update()
    cmds = tm.tick_update()
    assert "Look" in cmds
    assert "Inventory" not in cmds


@patch("ai.src.fsm.TickManager.INVENTORY_INTERVAL", 10)
@patch("ai.src.fsm.TickManager.LOOK_INTERVAL", 10)
@patch("ai.src.fsm.TickManager.BROADCAST_INTERVAL", 5)
def test_tick_should_broadcast_with_offset():
    tm = TickManager(agent_id=1)
    tm.tick_update()
    tm.tick_update()
    cmds = tm.tick_update()
    assert None in cmds


@patch("ai.src.fsm.TickManager.INVENTORY_INTERVAL", 2)
@patch("ai.src.fsm.TickManager.LOOK_INTERVAL", 2)
@patch("ai.src.fsm.TickManager.BROADCAST_INTERVAL", 2)
def test_tick_multiple_commands_simultaneously():
    tm = TickManager(agent_id=0)
    tm.tick_update()
    cmds = tm.tick_update()
    assert "Inventory" in cmds
    assert "Look" in cmds
    assert None in cmds
    assert len(cmds) == 3