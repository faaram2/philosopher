# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ayeganya <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/19 11:05:36 by ayeganya          #+#    #+#              #
#    Updated: 2024/10/02 15:13:15 by ayeganya         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Compiler and compiler CFLAGS
CC = cc
CFLAGS = -Wall -Werror -Wextra #-g3 -fsanitize=thread

# Header files
H_FILE = philosophers.h

#Source files
SRC_FILES = init.c philosophers.c utils.c
#B_SRC_FILES = 

OBJ_FILES = $(SRC_FILES:%.c=%.o)

#Target
NAME  = philo
#BONUS = fractol

all: $(LIBFT_A) $(NAME)

$(NAME): $(OBJ_FILES) $(H_FILE)
	$(CC) $(CFLAGS) $(OBJ_FILES) -o $@

#Object target
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

#cleaning targets
clean:
	rm -f $(OBJ_FILES)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
