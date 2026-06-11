#!/usr/bin/env python3
import sys
import os
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '.')))

from ai.src.mainIA import mainIA
def main():
    mainIA()

if __name__ == "__main__":
    main()
