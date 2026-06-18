#!/bin/bash

set -e

GREEN='\033[0;32m'
BLUE='\033[0;34m'
NC='\033[0m'

echo -e "${BLUE}[Tester] Checking Python environment...${NC}"

if [ ! -d ".venv" ]; then
    echo -e "${BLUE}[Tester] Creating virtual environment (.venv)...${NC}"
    python3 -m venv .venv
    
    echo -e "${BLUE}[Tester] Installing dependencies...${NC}"
    ./.venv/bin/python -m pip install -q --upgrade pip
    ./.venv/bin/python -m pip install -q -r requirements.txt
fi

echo -e "${GREEN}[Tester] Environment ready. Running tests...${NC}"

./.venv/bin/pytest "$@"
