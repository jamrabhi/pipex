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

SRC = src/pipex.c src/pipex_utils.c

OBJ = $(SRC:.c=.o)

INCDIR = include

LIBDIR = libft/

LIB = libft/libft.a

MAKEFLAGS += --no-print-directory

all: $(NAME)

$(NAME) : $(OBJ)
	@echo "Compiling Libft ..."
	@cd $(LIBDIR) && make
	@echo "DONE \n"

	@echo "Compiling Pipex ..."
	@$(CC) $(CFLAGS) $(OBJ) -L $(LIBDIR) -lft -o $(NAME)
	@echo "DONE"

.c.o:
	@${CC} ${CFLAGS} -I $(INCDIR) -I$(LIBDIR) -c $< -o $@

clean:
	@echo "Deleting Libft objects files ..."
	@cd $(LIBDIR) && make $@
	@echo "DONE \n"

	@echo "Deleting Pipex objects files ..."
	@rm -f $(OBJ)
	@echo "DONE"

fclean: clean

	@echo "Deleting Libft's binary ..."
	@cd $(LIBDIR) && make $@
	@echo "DONE \n"
	
	@echo "Deleting Pipex's binary ..."
	@rm -f $(NAME)
	@echo "DONE"

re: fclean all