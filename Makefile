# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jamrabhi <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/03/22 18:32:05 by jamrabhi          #+#    #+#              #
#    Updated: 2022/03/22 18:32:09 by jamrabhi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex

CC = gcc

CFLAGS = -Wall -Wextra -Werror

SRC = src/pipex.c

OBJ = $(SRC:.c=.o)

INCDIR = include

LIBDIR = lib/libft/

LIB = lib/libft/libft.a

MAKEFLAGS += --no-print-directory

all: $(NAME)

$(NAME) : $(OBJ)
	@echo "Compiling Libft ..."
	@cd $(LIBDIR) && make
	@echo "DONE \n"

	@echo "Compiling Pipex ..."
	@$(CC) $(CFLAGS) $(OBJ) -L $(LIBDIR) -lft -o $(NAME)
	@echo "DONE \n"

.c.o:
	@${CC} ${CFLAGS} -I $(INCDIR) -I$(LIBDIR) -c $< -o $@

debug: $(OBJ)
	@echo "Compiling Libft ..."
	@cd $(LIBDIR) && make
	@echo "DONE \n"

	@echo "Compiling Pipex with fsanitize ..."
	@$(CC) $(CFLAGS) $(OBJ) -g3 -fsanitize=address -L $(LIBDIR) -lft -o $(NAME)
	@echo "DONE \n"

clean:
	@echo "Deleting Libft objects files ..."
	@cd $(LIBDIR) && make $@
	@echo "DONE \n"

	@echo "Deleting Pipex objects files ..."
	@rm -f $(OBJ)
	@echo "DONE \n"

fclean: clean

	@echo "Deleting Libft's binary ..."
	@cd $(LIBDIR) && make $@
	@echo "DONE \n"
	
	@echo "Deleting Pipex's binary ..."
	@rm -f $(NAME)
	@echo "DONE \n"

re: fclean all

redebug: fclean debug