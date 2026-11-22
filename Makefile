NAME := codexion

CC := cc

CFLAGS := -Wall -Wextra -Werror
SRCS := main.c args_validation.c coder.c supervisor.c time_utils.c

OBJ := $(SRCS:.c=.o)

ARGS := 3 150 100 100 100 4 30 fifo

all: $(NAME)

run:
	./$(NAME) $(ARGS)

memcheck:
	valgrind --leak-check=full --show-leak-kinds=all ./$(NAME) $(ARGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) 
clear:
	rm -f $(OBJ) $(NAME)

fclean: clear
	rm -f $(NAME)

re: fclean all

.PHONY: all clear fclean re
.DEFAULT_GOAL := all
