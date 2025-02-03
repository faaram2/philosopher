# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ayeganya <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/19 11:05:36 by ayeganya          #+#    #+#              #
#    Updated: 2024/10/02 15:18:12 by ayeganya         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Compiler and compiler CFLAGS
CC = cc
CFLAGS = -Wall -Werror -Wextra

# Header files
H_FILE = philosophers.h

#Source files
SRC_FILES = init.c philosophers.c utils.c utils2.c
#B_SRC_FILES = 

OBJ_FILES = $(SRC_FILES:%.c=%.o)

#Target
NAME  = philo_bonus

all: $(LIBFT_A) $(NAME)

$(NAME): $(OBJ_FILES) $(H_FILE)
	$(CC) $(CFLAGS) $(OBJ_FILES) $(LIBFT_A) -o $@

#Object target
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

#Libft target
$(LIBFT_A):
	$(MAKE) -C $(LIBFT_DIR)

#cleaning targets
clean:
	rm -f $(OBJ_FILES)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
