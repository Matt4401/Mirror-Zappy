#!/usr/bin/env python3
import sys
import argparse
from AITeamClass import AITeam
from TrantorianClass import Trantorian

def main():
    parser = argparse.ArgumentParser(
        usage="./zappy_ai -p port -n name -h machine",
        add_help=False
    )
    parser.add_value_provider = False
    parser.add_argument('--help', action='help', help='Show this help message -> exit')
    parser.add_argument('-p', type=int, required=True, metavar='port')
    parser.add_argument('-n', type=str, required=True, metavar='name')
    parser.add_argument('-h', type=str, required=True, metavar='machine')

    try:
        args = parser.parse_args()
    except SystemExit:
        sys.exit(84)
    port = args.p
    name = args.n
    machine = args.h

    trantorian = Trantorian("matt4401")
    main_class = AITeam(trantorian, machine)
    #main_class.run()


if __name__ == "__main__":
    main()