CC		= cc
CFLAGS	= -Wall -Wextra -Werror

FILES	= philo.c routine.c monitoring.c utils/utils00.c utils/utils01.c
OBJ		= $(FILES:.c=.o)

NAME	= philo

HEADER	= philo.h

all: $(NAME)

$(NAME): $(OBJ) $(HEADER)
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re