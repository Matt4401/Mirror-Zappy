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
    parser.add_argument("--help", action="help", help="Show this help message and exit")
    parser.add_argument("-p", type=int, required=True, metavar="port")
    parser.add_argument("-n", type=str, required=True, metavar="name")
    parser.add_argument(
        "-h", type=str, required=True, metavar="machine", dest="machine"
    )

    try:
        args = parser.parse_args()
    except SystemExit:
        sys.exit(84)
    port = args.p
    team_name = args.n
    host = args.machine

    player_id = generate_id(team_name)
    PlayerLogger.setup_logging(player_id)
    print(f"Logger is ok: {player_id}")

    trantorian = None
    try:
        trantorian = Trantorian(port, host, team_name, player_id)
        print("Trantorian ok")
        tick_manager = TickManager(10)
        fsm = FiniteStateMachine(SurviveState, trantorian, tick_manager)
        print("fsm ok")
        fsm.run()

    except KeyboardInterrupt:
        if trantorian is not None:
            trantorian.connection.disconnect()
        sys.exit(0)


if __name__ == "__main__":
    mainAI()
