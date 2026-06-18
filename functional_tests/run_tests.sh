#!/bin/bash

GREEN='\033[0;32m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
YELLOW='\033[1;33m'
NC='\033[0m'

echo -e "${BLUE}[Tester] Checking Python environment...${NC}"

if [ ! -d ".venv" ]; then
    echo -e "${BLUE}[Tester] Creating virtual environment (.venv)...${NC}"
    python3 -m venv .venv
    ./.venv/bin/python -m pip install -q --upgrade pip
    ./.venv/bin/python -m pip install -q -r requirements.txt
fi

set +e

echo ""
echo -e "${PURPLE}======================================================================${NC}"
echo -e "${PURPLE}                  [1/2] TESTS REFERENCE BINARY                        ${NC}"
echo -e "${PURPLE}======================================================================${NC}"
./.venv/bin/pytest "$@" -k "reference"
REF_STATUS=$?

echo ""
echo -e "${YELLOW}======================================================================${NC}"
echo -e "${YELLOW}                  [2/2] TESTS CUSTOM BINARY                           ${NC}"
echo -e "${YELLOW}======================================================================${NC}"
./.venv/bin/pytest "$@" -k "custom"
CUST_STATUS=$?

set -e

echo ""
if [ $REF_STATUS -ne 0 ] || [ $CUST_STATUS -ne 0 ]; then
    echo -e "${BLUE}[Tester] End of tests. Errors recorded.${NC}"
    exit 1
else
    echo -e "${GREEN}[Tester] Tests passed !${NC}"
    exit 0
fi
