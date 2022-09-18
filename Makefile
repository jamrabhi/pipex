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

CFLAGS = -Wall -Wextra -Werror -g

SRC = mandatory/src/pipex.c mandatory/src/pipex_utils.c

SRC_BONUS = bonus/src/pipex_bonus.c bonus/src/pipex_utils_bonus.c

OBJ = $(SRC:.c=.o)

OBJ_BONUS = $(SRC_BONUS:.c=.o)

INCDIR = mandatory/include/

INCDIR_BONUS = bonus/include/

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


bonus: $(OBJ_BONUS)
	@make OBJ="$(OBJ_BONUS)" all

.c.o:
	@${CC} ${CFLAGS} -I $(INCDIR) -I $(INCDIR_BONUS) -I $(LIBDIR) -c $< -o $@

# .c.o:
# 	@${CC} ${CFLAGS} -I $(INCDIR_BONUS) -I$(LIBDIR) -c $< -o $@

clean:
	@echo "Deleting Libft objects files ..."
	@cd $(LIBDIR) && make $@
	@echo "DONE \n"

	@echo "Deleting Pipex objects files ..."
	@rm -f $(OBJ) $(OBJ_BONUS)
	@echo "DONE"

fclean: clean

	@echo "Deleting Libft's binary ..."
	@cd $(LIBDIR) && make $@
	@echo "DONE \n"
	
	@echo "Deleting Pipex's binary ..."
	@rm -f $(NAME) $(BONUS)
	@echo "DONE"

re: fclean all