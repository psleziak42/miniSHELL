# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: psleziak <psleziak@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/09/28 18:49:28 by tosilva           #+#    #+#              #
#    Updated: 2021/11/11 20:00:13 by psleziak         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

################################################################################
### Variables																 ###
################################################################################

########################################
## Target Name						  ##
########################################

NAME	= minishell
BONUS	=


########################################
## Folders & Files					  ##
########################################

DEBUG_FILES	= $(NAME).dSYM/

# DEFAULT FOLDERS
INC_DIR	= include/
SRC_DIR	= src/
OBJ_DIR	= obj/
DEP_DIR	= dep/
SRC_DIR_BONUS	= src_bonus/
OBJ_DIR_BONUS	= obj_bonus/
DEP_DIR_BONUS	= dep_bonus/

# LIBRARIES
LIBFT_DIR	= libs/libft/
LIBFT_FILE	= libft.a
LIBFT		:= ${addprefix $(LIBFT_DIR), $(LIBFT_FILE)}

# FILES
HFILES	:= minishell.h
CFILES	:= check_commands.c \
			error_clear.c \
			fill_builtins_struct.c \
			ft_cd.c \
			ft_exit_echo_pwd_env.c \
			ft_expand_dollar_utils.c \
			ft_expand_dollar.c \
			ft_expand_tilde_utils.c \
			ft_expand_tilde.c \
			ft_export_n_unset.c \
			ft_parser_copy_args.c \
			ft_parser_utils.c \
			ft_parser.c \
			lst_arguments.c \
			minishell_initialization.c \
			minishell.c \
			pipes_and_single_cmd_utils.c \
			pipes.c \
			redirections.c \
			run_builtin_or_execve.c \
			run_pipe_or_single_cmd.c

HFILES_BONUS	:=
CFILES_BONUS	:=
	
INC	:= ${foreach file, $(INC_DIR), ${addprefix $(INC_DIR), $(HFILES)}}
SRC	:= ${foreach file, $(SRC_DIR), ${addprefix $(SRC_DIR), $(CFILES)}}
OBJ	:= ${subst $(SRC_DIR), $(OBJ_DIR), $(SRC:.c=.o)}
DEP	:= ${subst $(SRC_DIR), $(DEP_DIR), $(SRC:.c=.d)}

INC_BONUS	:= ${foreach file, $(INC_DIR), ${addprefix $(INC_DIR), $(HFILES_BONUS)}}
SRC_BONUS	:= ${foreach file, $(SRC_DIR_BONUS), ${addprefix $(SRC_DIR_BONUS), $(CFILES_BONUS)}}
OBJ_BONUS	:= ${subst $(SRC_DIR_BONUS), $(OBJ_DIR_BONUS), $(SRC_BONUS:.c=.o)}
DEP_BONUS	:= ${subst $(SRC_DIR_BONUS), $(DEP_DIR_BONUS), $(SRC_BONUS:.c=.d)}


########################################
## Colors							  ##
########################################

# Reset
COLOR_RESET		=[0m

COLORF_RED		=[31m
COLORF_GREEN	=[32m
COLORF_YELLOW	=[33m
COLORF_CYAN		=[36m

CL_NORM_OK		:=$(COLORF_GREEN)
CL_NORM_WARN	:=$(COLORF_YELLOW)
CL_NORM_ERROR	:=$(COLORF_RED)
CL_NORM_NUM		:=$(COLORF_CYAN)


########################################
## Commands & Flags					  ##
########################################

CC			= gcc
CFLAGS		= -Wall -Werror -Wextra # -03
DEBFLAGS	= -g -fsanitize=address
INCFLAGS	:= -I./$(LIBFT_DIR) -I./$(INC_DIR) -I ~/.brew/opt/readline/include#'pkg-config readline --cflags'
LIBFLAGS	:= -L./$(LIBFT_DIR) -lft -L ~/.brew/opt/readline/lib -lreadline
#PKGFLAGS	= 'pkg-config readline --cflags' 'pkg-config readline --libs'

NORM		= norminette
NORM_COLORS	:= sed "s/OK!/$(CL_NORM_OK)OK!$/$(COLOR_RESET)/g"
NORM_COLORS	+= | sed -E "s/Error(!|:)/$(CL_NORM_ERROR)Error\1$/$(COLOR_RESET)/g"
NORM_COLORS	+= | sed -E "s/Warning(!|:)/$(CL_NORM_WARN)Warning\1$/$(COLOR_RESET)/g"
NORM_COLORS	+= | sed -E "s/line:[[:blank:]]+([0-9]+)/$(CL_NORM_NUM)\1$/$(COLOR_RESET)/g"
NORM_COLORS	+= | sed -E "s/col:[[:blank:]]+([0-9]+)/$(CL_NORM_NUM)\1$/$(COLOR_RESET)/g"

MKDIR	= mkdir -p
RM		= rm -fr



################################################################################
### Rules																	 ###
################################################################################

.DELETE_ON_ERROR:

########################################
## Compile							  ##
########################################

all: libft target bonus

libft $(LIBFT): | $(LIBFT_DIR)
	@echo "[ .. ] Creating LIBFT [ .. ]"
	@$(MAKE) -C $(LIBFT_DIR)
	@echo "\n\n$(COLORF_GREEN)"
	@cat ./fonts/libft_created.txt
	@echo "$(COLOR_RESET)"

target: print_create $(NAME) print_created

print_create:
	@echo "\n"
	@echo "[ .. ] Creating '$(NAME)' [ .. ]"

print_created:
	@echo "\n$(COLORF_GREEN)"
	@cat ./fonts/minishell_created.txt
	@echo "$(COLOR_RESET)"

$(NAME): $(LIBFT) $(OBJ)
	@$(CC) $(CFLAGS) $(PKGFLAGS) $(INCFLAGS) $^ $(LIBFLAGS) -o $@

$(OBJ): $(OBJ_DIR)%.o: $(SRC_DIR)%.c | $(DEP_DIR)%.d $(OBJ_DIR)
	@$(CC) $(CFLAGS) $(INCFLAGS) -c $< -o $@
	@echo "$(COLORF_GREEN)#$(COLOR_RESET)\c"

$(DEP): $(DEP_DIR)%.d: $(SRC_DIR)%.c $(LIBFT) | $(DEP_DIR)
	@$(CC) $(CFLAGS) $(INCFLAGS) -M \
		-MT '${patsubst $(SRC_DIR)%.c,$(OBJ_DIR)%.o,$<} \
			${patsubst $(SRC_DIR)%.c,$(DEP_DIR)%.d,$<}' $< \
		-MF ${patsubst $(SRC_DIR)%.c,$(DEP_DIR)%.d,$<}

bonus: $(BONUS)

$(BONUS): $(LIBFT) $(OBJ_BONUS)
	$(CC) $(CFLAGS) $(INCFLAGS) $^ $(LIBFLAGS) -o $@

$(OBJ_BONUS): $(OBJ_DIR_BONUS)%.o: $(SRC_DIR_BONUS)%.c | $(DEP_DIR_BONUS)%.d $(OBJ_DIR_BONUS)
	$(CC) $(CFLAGS) $(INCFLAGS) -c $< -o $@

$(DEP_BONUS): $(DEP_DIR_BONUS)%.d: $(SRC_DIR_BONUS)%.c $(LIBFT) | $(DEP_DIR_BONUS)
	@$(CC) $(CFLAGS) $(INCFLAGS) -M \
		-MT '${patsubst $(SRC_DIR_BONUS)%.c,$(OBJ_DIR_BONUS)%.o,$<} \
			${patsubst $(SRC_DIR_BONUS)%.c,$(DEP_DIR_BONUS)%.d,$<}' $< \
		-MF ${patsubst $(SRC_DIR_BONUS)%.c,$(DEP_DIR_BONUS)%.d,$<}

########################################
## Debug							  ##
########################################

debug: CFLAGS += $(DEBFLAGS)
debug: debug_libft all

debug_libft: libft


########################################
## Make dirs						  ##
########################################

$(OBJ_DIR) $(DEP_DIR):
	@$(MKDIR) $@

$(OBJ_DIR_BONUS) $(DEP_DIR_BONUS):
	@$(MKDIR) $@


########################################
## Norm								  ##
########################################

norm:
	@echo "[ .. ] Norminette [ .. ]"
	@echo
	@echo "»»»»» HEADERS «««««"
	@$(NORM) $(INC) | $(NORM_COLORS)
	@echo 
	@echo "»»»»» SOURCES  «««««"
	@$(NORM) $(SRC) | $(NORM_COLORS)
	@echo

norm_bonus:
	@echo "[ .. ] Norminette BONUS [ .. ]"
	@echo "»»»»» HEADERS «««««"
	@$(NORM) $(INC_BONUS) | $(NORM_COLORS)
	@echo 
	@echo "»»»»» SOURCES  «««««"
	@$(NORM) $(SRC_BONUS) | $(NORM_COLORS)
	@echo

norm_libft:
	@echo "[ .. ] Norminette LIBFT [ .. ]"
	@$(NORM) $(LIBFT_DIR) | $(NORM_COLORS)


########################################
## Clear							  ##
########################################

clean:
	@echo "[ .. ] Deleting LIBFT [ .. ]"
	@$(MAKE) $@ -C $(LIBFT_DIR)
	@echo "$(COLORF_RED)"
	@cat ./fonts/libft_deleted.txt
	@echo "$(COLOR_RESET)"
	@echo "[ .. ] Deleting .o files [ .. ]"
	@$(RM) $(OBJ_DIR) $(OBJ_DIR_BONUS)

fclean: clean
	@echo "[ .. ] Deleting '$(NAME)' executable  [ .. ]"
	@$(RM) $(BIN_DIR) $(NAME) $(BONUS)
	@echo "$(COLORF_RED)"
	@cat	./fonts/minishell_deleted.txt
	@echo "$(COLOR_RESET)"

clean_dep:
	@echo "[ .. ] Deleting prerequisites files  [ .. ]"
	@$(RM) $(DEP_DIR) $(DEP_DIR_BONUS)

clean_debug:
	@echo "[ .. ] Deleting debug files  [ .. ]"
	@$(RM) $(DEBUG_FILES)

clean_all: fclean clean_debug clean_dep


########################################
## Reload							  ##
########################################

re_debug: fclean debug

re: fclean all



################################################################################
### Phony																	 ###
################################################################################

.PHONY: all libft bonus target print_create print_created debug
.PHONY: norm norm_libft
.PHONY: clean fclean clean_dep clean_debug clean_all
.PHONY: re_debug re



################################################################################
### Includes																 ###
################################################################################

-include ${shell find . -maxdepth 2 -type f -name '*.d'}
