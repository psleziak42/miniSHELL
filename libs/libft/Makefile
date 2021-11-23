# COLORS

GREEN	=	\033[0;32m
RESET	=	\033[0m

SRCS	=	$(wildcard *.c)

OBJS	= $(SRCS:.c=.o)

NAME	= libft.a

CC = gcc
RM = rm -f

CFLAGS = -Wall -Wextra -Werror

.c.o:
	@$(CC) $(CFLAGS) -c $< -o $(<:.c=.o)
	@echo "$(GREEN)#$(RESET)\c"

$(NAME): $(OBJS)
	@ar rcs $(NAME) $(OBJS)

all:	$(NAME)

clean:
	@$(RM) $(OBJS)

fclean:	clean
	@$(RM) $(NAME)

re: fclean all

so:
	$(CC) -fPIC $(CFLAGS) $(SRC)
	gcc -shared -o libft.so $(OBJ)
