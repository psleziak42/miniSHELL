# COLORS

GREEN	= \033[0;32m
RED		= \033[0;31m
BLUE	= \033[0;34m
GREY	= \033[38;5;240m
WHITE	= \033[0;37m
QUIT	= \033[0m

# MACROS

NAME	=	minishell
SRCS	=	cfiles/minishell.c cfiles/minishell_builtins.c
DIR_O	=	OBJ
OBJS	=	$(SRCS:%.c=%.o)
DOTH	=	extras/hfiles
LIBFT	=	extras/libft
LFT_EXE	=	extras/libft
CC		=	gcc
CFLAGS	=	-Wall -Wextra -Werror
LDFLAGS = 	-g #-fsanitize=address
INCLUDE	=	-I./$(DOTH) -I./$(LIBFT)
LINKS	=	-L./$(LIBFT) -lft -lreadline

# RULES

all:	$(NAME)


$(NAME): libft $(OBJS)
	@$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) $(LINKS) -o $(NAME)
	@mkdir -p $(DIR_O)
	@mv cfiles/*.o OBJ
	@echo "\n$(GREEN)\n"
	@cat ./fonts/minishell_created.txt
	@echo "\n$(RESET)\n"

%.o: %.c
	@$(CC) -c $(CFLAGS) $(INCLUDE) $< -o $@
	@echo "$(GREEN)#$(RESET)\c"

libft:
	@echo "$(WHITE) [ .. ] Creating LIBFT [ .. ]$(RESET)"
	@make -C $(LIBFT)
	@echo "\n$(GREEN)\n"
	@cat ./fonts/libft_created.txt
	@echo "$(WHITE) \n\n\n[ .. ] Creating '$(NAME)' [ .. ]$(RESET)"

clean:
	@echo "$(RED) [ .. ] Deleting LIBFT [ .. ]"
	@echo "$(RED)"
	@make -C $(LIBFT) fclean
	@cat ./fonts/libft_deleted.txt
	@echo "$(RESET)"
	@echo "$(RED) [ .. ] Deleting .o files [ .. ]$(RESET)"
	@rm -fr $(DIR_O)

fclean:	clean
	@echo "$(RED) [ .. ] Deleting '$(NAME)' executable  [ .. ]$(RESET)"
	@rm -fr $(NAME)
	@cat	./fonts/minishell_deleted.txt
	@echo "$(RESET)"

re:		fclean all

.PHONY	=	all clean fclean re
