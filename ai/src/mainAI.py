import sys
import argparse
from src.AITeamClass import *
from src.TrantorianClass import *


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

    trantorian = Trantorian("matt4401")
    main_class = AITeam(trantorian, port, machine, name)  # noqa: F841
    main_class.run()
