##
## EPITECH PROJECT, 2026
## Zappy
## File description:
## Makefile
##

BUILD_DIR	=	build

NPROC := $(shell nproc)
CORES := $(shell expr $(NPROC) - 1)
ifeq ($(CORES),0)
CORES := 1
endif

all:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake .. && $(MAKE) -j$(CORES)

zappy_server:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake .. && $(MAKE) zappy_server -j$(CORES)

zappy_gui:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake .. && $(MAKE) zappy_gui -j$(CORES)

zappy_ai:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake .. && $(MAKE) zappy_ai -j$(CORES)

clean:
	@rm -rf $(BUILD_DIR)
	@rm -rf lib

fclean: clean
	@rm -f zappy_server
	@rm -f zappy_gui
	@rm -f zappy_ai

re: fclean all

.PHONY: all zappy_server zappy_gui zappy_ai clean fclean re
