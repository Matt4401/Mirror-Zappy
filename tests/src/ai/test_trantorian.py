import pytest
from unittest.mock import MagicMock, patch
from src.Trantorian import Trantorian


@pytest.fixture
def mock_dependencies():
    with patch("src.Trantorian.Connection") as mock_conn, \
            patch("src.Trantorian.PlayerState") as mock_state, \
            patch("src.Trantorian.SendCommand") as mock_send, \
            patch("src.Trantorian.ParseCommand") as mock_parse, \
            patch("src.Trantorian.BroadcastMessage") as mock_broadcast:

        conn_instance = MagicMock()
        mock_conn.return_value = conn_instance

        state_instance = MagicMock()
        mock_state.return_value = state_instance

        send_instance = MagicMock()
        mock_send.return_value = send_instance

        yield {
            "connection": conn_instance,
            "player_state": state_instance,
            "send_command": send_instance,
        }


@pytest.fixture
def trantorian(mock_dependencies):
    return Trantorian(port=1234, host="localhost", team_name="TeamA", player_id="player_1", bonus=False)


def test_trantorian_gender_without_bonus(mock_dependencies):
    t = Trantorian(port=1234, host="localhost", team_name="TeamA", player_id="player_1", bonus=False)
    assert t.sex is None


def test_trantorian_gender_with_bonus(mock_dependencies):
    with patch("random.randint") as mock_rand:
        mock_rand.return_value = 1
        t_male = Trantorian(port=1234, host="localhost", team_name="TeamA", player_id="player_1", bonus=True)
        assert t_male.sex == "Male"

        mock_rand.return_value = 0
        t_female = Trantorian(port=1234, host="localhost", team_name="TeamA", player_id="player_1", bonus=True)
        assert t_female.sex == "Female"



def test_wait_for_response_success(trantorian, mock_dependencies):
    mock_dependencies["connection"].get_command_response.return_value = (True, "ok")
    result = trantorian.wait_for_response(1)
    assert result == (True, "ok")


def test_wait_for_response_invalid_ids(trantorian):
    assert trantorian.wait_for_response(84) is None
    assert trantorian.wait_for_response(None) is None



def test_forward_success(trantorian, mock_dependencies):
    mock_dependencies["send_command"].forward.return_value = 42
    mock_dependencies["connection"].get_command_response.return_value = (True, "ok")

    result = trantorian.forward()

    assert result == (True, "ok")
    mock_dependencies["player_state"].vision.reset_on_turn.assert_called_once()


def test_turn_right_updates_direction(trantorian, mock_dependencies):
    mock_dependencies["send_command"].right.return_value = 10
    mock_dependencies["connection"].get_command_response.return_value = (True, "ok")

    mock_dependencies["player_state"].get_direction.return_value = 1

    result = trantorian.turn_right()

    assert result == (True, "ok")
    mock_dependencies["player_state"].update_direction.assert_called_once_with(2)
    mock_dependencies["player_state"].vision.reset_on_turn.assert_called_once()


def test_turn_left_handles_wrap_around(trantorian, mock_dependencies):
    mock_dependencies["send_command"].left.return_value = 11
    mock_dependencies["connection"].get_command_response.return_value = (True, "ok")

    mock_dependencies["player_state"].get_direction.return_value = 1

    result = trantorian.turn_left()

    assert result == (True, "ok")
    mock_dependencies["player_state"].update_direction.assert_called_once_with(8)


def test_take_object_success(trantorian, mock_dependencies):
    mock_dependencies["send_command"].take_object.return_value = 5
    mock_dependencies["connection"].get_command_response.return_value = (True, "ok")

    result = trantorian.take_object("linemate")

    assert result == (True, "ok")
    mock_dependencies["send_command"].take_object.assert_called_once_with("linemate")
    mock_dependencies["player_state"].vision.reset_on_turn.assert_called_once()



def test_has_enough_resources(trantorian, mock_dependencies):
    with patch("src.Trantorian.ELEVATION_REQUIREMENTS", {2: {"linemate": 1, "deraumere": 0}}):
        mock_dependencies["player_state"].inventory.linemate = 2
        mock_dependencies["player_state"].inventory.deraumere = 0

        assert trantorian.has_enough_resources_for(2) is True


def test_has_enough_resources_insufficient(trantorian, mock_dependencies):
    with patch("src.Trantorian.ELEVATION_REQUIREMENTS", {2: {"linemate": 2}}):
        mock_dependencies["player_state"].inventory.linemate = 1

        assert trantorian.has_enough_resources_for(2) is False


def test_get_missing_resources(trantorian, mock_dependencies):
    with patch("src.Trantorian.ELEVATION_REQUIREMENTS", {3: {"linemate": 2, "sibur": 1}}):
        mock_dependencies["player_state"].inventory.linemate = 0
        mock_dependencies["player_state"].inventory.sibur = 1

        missing = trantorian.get_missing_resources_for(3)

        assert missing == {"linemate": 2}