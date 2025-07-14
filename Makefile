NAME = philo
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g3 #-pthread -g3 -fsanitize=address

SRCS = philo.c routine.c monitor.c parse_args.c help_routine.c cleanup.c
OBJS = $(SRCS:.c=.o)
DEPS = philo.h

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re