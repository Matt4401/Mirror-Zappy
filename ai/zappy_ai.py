#!/usr/bin/env python3
import sys
import os

# 1. On trouve le chemin absolu du dossier "ai" (où se trouve ce fichier zappy_ai.py)
ai_dir = os.path.dirname(os.path.abspath(__file__))

# 2. On le pousse au TOUT DÉBUT du sys.path pour que Python cherche en priorité dedans
if ai_dir not in sys.path:
    sys.path.insert(0, ai_dir)

# Maintenant, Python sait que "src" est juste à côté, même s'il est exécuté depuis l'autre bout du PC
from src.mainAI import mainAI

def main():
    return mainAI()

if __name__ == "__main__":
    main()