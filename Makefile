# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sichoi <sichoi@student.42seoul.kr>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/05/13 14:52:17 by sichoi            #+#    #+#              #
#    Updated: 2022/05/18 16:51:20 by sichoi           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
LIB_NAME = readline
LIB_DIR = $(shell brew --prefix readline)/lib/
LIB_INC = $(shell brew --prefix readline)/include/
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
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ) -l $(LIB_NAME) -I $(LIB_INC) -L $(LIB_DIR)
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
