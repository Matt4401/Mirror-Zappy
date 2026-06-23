import sys
import argparse
from src.Trantorian import Trantorian
from util.IdGenerator import generate_id
from util.InitLogger import PlayerLogger


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

    main_class = Trantorian(trantorian, port, machine, name)  # noqa: F841
    main_class.run()
