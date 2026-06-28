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
    parser.add_argument("-b", action="store_true", help="get bonus mode")
    parser.add_argument("-g", type=int, default=0, metavar="generation")

    try:
        args = parser.parse_args()
    except SystemExit:
        sys.exit(84)
    port = args.p
    team_name = args.n
    host = args.machine
    generation = args.g

    player_id = generate_id(team_name)
    PlayerLogger.setup_logging(player_id, fresh=(generation == 0))
    print(f"Logger is ok: {player_id}")

    trantorian = None
    try:
        connect_timeout = 15.0 if generation > 0 else 3.0
        trantorian = Trantorian(
            port, host, team_name, player_id, args.b, connect_timeout
        )
        trantorian.generation = generation
        print("Trantorian ok")
        if generation == 0:
            trantorian.fill_initial_team()
        tick_seed = sum(ord(char) for char in player_id)
        tick_manager = TickManager(tick_seed)
        fsm = FiniteStateMachine(SurviveState, trantorian, tick_manager)
        print("fsm ok")
        fsm.run()

    except KeyboardInterrupt:
        if trantorian is not None:
            trantorian.connection.disconnect()
        sys.exit(0)


if __name__ == "__main__":
    mainAI()
