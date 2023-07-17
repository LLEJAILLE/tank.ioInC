##
## EPITECH PROJECT, 2023
## B-NWP-400-NCY-4-1-myteams-yoan.gerard
## File description:
## Makefile
##

SRC = $(wildcard *.c)

CFLAGS = -I./includes

NAME = tank_server

NO_COLOR = \x1b[0m
OK_COLOR = \x1b[32;01m

OBJDIR = .obj

OBJ = ${patsubst %.c, ${OBJDIR}/%.o, ${SRC}}

${OBJDIR}/%.o: %.c
	@echo -e "${OK_COLOR}Compiling $<...${NO_COLOR}"
	@mkdir -p ${@D}
	gcc ${CFLAGS} -c $< -o $@

all: $(NAME)

$(NAME): $(OBJ)
	gcc -o $(NAME) $(OBJ) $(CFLAGS) -luuid -g -lm
	mv $(NAME) ../

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f ../$(NAME)

re: fclean all

.PHONY: all clean fclean re
