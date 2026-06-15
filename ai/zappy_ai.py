#!/usr/bin/env python3
# ruff: noqa: F401
import sys
import os


ai_dir = os.path.dirname(os.path.abspath(__file__))

if ai_dir not in sys.path:
    sys.path.insert(0, ai_dir)
from src.mainAI import mainAI  # noqa: E402


def main():
    return mainAI()


if __name__ == "__main__":
    main()
