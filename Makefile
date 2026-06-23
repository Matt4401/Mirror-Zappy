##
## EPITECH PROJECT, 2026
## Zappy
## File description:
## Makefile
##

BUILD_DIR	=	build

all:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake .. && $(MAKE) -j$(shell nproc)

zappy_server:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake .. && $(MAKE) zappy_server -j$(shell nproc)

zappy_gui:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake .. && $(MAKE) zappy_gui -j$(shell nproc)

zappy_ai:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake .. && $(MAKE) zappy_ai -j$(shell nproc)

clean:
	@rm -rf $(BUILD_DIR)
	@rm -rf lib

fclean: clean
	@rm -f zappy_server
	@rm -f zappy_gui
	@rm -f zappy_ai

re: fclean all

.PHONY: all zappy_server zappy_gui zappy_ai clean fclean re
