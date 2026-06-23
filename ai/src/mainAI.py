import sys
import argparse
from src.Trantorian import Trantorian
from src.util.IdGenerator import generate_id
from src.util.InitLogger import PlayerLogger
from src.fsm.TickManager import TickManager
from src.fsm.FSM import FiniteStateMachine
from src.fsm.states.SurviveState import SurviveState


def mainAI():
    parser = argparse.ArgumentParser(
        usage="./zappy_ai -p port -n name -h machine", add_help=False
    )
    parser.add_value_provider = False
    parser.add_argument("--help", action="help", help="Show this help message -> exit")
    parser.add_argument("-p", type=int, required=True, metavar="port")
    parser.add_argument("-n", type=str, required=True, metavar="name")
    parser.add_argument("-h", type=str, required=True, metavar="machine")

    try:
        args = parser.parse_args()
    except SystemExit:
        sys.exit(84)
    port = args.p  # noqa: F841
    name = args.n  # noqa: F841
    machine = args.h

    # TEMPORARY !!!!!!!!
    team_name = "test"
    player_id = generate_id(team_name)

    PlayerLogger.setup_logging(player_id)

    trantorian = Trantorian(port, machine, name)  # noqa: F841
    print("trantorian is readyy")

    id = generate_id(trantorian.team_name)

    tick_manager = TickManager(10)
    fsm = FiniteStateMachine(SurviveState, trantorian, tick_manager)
    fsm.run()
