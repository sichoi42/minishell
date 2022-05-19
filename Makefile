# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sichoi <sichoi@student.42seoul.kr>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/05/13 14:52:17 by sichoi            #+#    #+#              #
#    Updated: 2022/05/19 16:47:31 by sichoi           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
LIB_NAME = readline
# LIB_DIR = $(shell brew --prefix readline)/lib/
# LIB_INC = $(shell brew --prefix readline)/include/

LIB_DIR = /opt/homebrew/Cellar/readline/8.1.2/lib/
LIB_INC = /opt/homebrew/Cellar/readline/8.1.2/include/
GREEN = \033[0;32m
RED = \033[0;31m
RESET = \033[0m
CHECK = \033[0;32m\xE2\x9C\x94\033[0m

FUNC = ./tokenize/print_token_info\
		./tokenize/token_utils\
		./tokenize/tokenize\
		./tokenize/tokenize_dollar_in_word\
		./tokenize/tokenize_one_elem\
		./tokenize/tokenize_quote_in_word\
		./tokenize/tokenize_redirect\
		./tokenize/tokenize_word\
		./parsing/parse_utils\
		./parsing/parsing\
		./parsing/syntax_analysis\
		./term/cursor\
		./term/termios\
		basic\
		builtin\
		env\
		free\
		input_env\
		main\
		pipe\
		redirect\
		utils\
		ft_itoa\

SRC = $(addsuffix .c, $(FUNC))
OBJ = $(addsuffix .o, $(FUNC))

%.o : %.c
	@echo "$(NAME)$$ $(GREEN) compiling... $< $(CHECK) $(RESET)"
	@$(CC) $(CFLAGS) -o $@ -c $< -I $(LIB_INC)

$(NAME) : $(OBJ)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ) -l $(LIB_NAME) -lcurses -I $(LIB_INC) -L $(LIB_DIR)
	@echo "$(NAME)$$ $(GREEN) $(NAME) was created! $(RESET)"

all : $(NAME)

norm :
	Norminette tokenize/*.c parsing/*.c *.c

clean :
	@rm -f $(OBJ)
	@echo "$(NAME)$$ $(RED) object files were deleted $(RESET)"

fclean : clean
	@rm -f $(NAME)
	@echo "$(NAME)$$ $(RED) $(NAME) was deleted $(RESET)"

re: fclean all
