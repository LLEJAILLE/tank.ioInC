##
## EPITECH PROJECT, 2023
## B-NWP-400-NCY-4-1-myteams-yoan.gerard
## File description:
## Makefile
##

NO_COLOR = \x1b[0m
OK_COLOR = \x1b[32;01m

all:
	@echo -e "${OK_COLOR}Compiling client and server...${NO_COLOR}"

	@make -C client
	@make -C server

clean:
	@make clean -s -C client
	@make clean -s -C server

fclean: clean
	@make fclean -s -C client
	@make fclean -s -C server

re: fclean all
