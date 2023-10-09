# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dgutak <dgutak@student.42vienna.com>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/31 17:55:20 by dgutak            #+#    #+#              #
#    Updated: 2023/10/09 17:07:59 by dgutak           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -L./libft -lft

NAME = philo
LIBFT	= ./libft/libft.a
SRCS = main.c utils.c error.c action.c routine.c

OBJS = $(SRCS:.c=.o)


all: $(NAME)

$(NAME): $(OBJS)
	make -C ./libft/
	$(CC) $(CFLAGS) $(LIBFT) -o $(NAME) $(OBJS) $(LDFLAGS) -g

.c.o:
	cc $(CFLAGS) -c $< -o $(<:.c=.o) -I ./include -I ./libft


clean:
	rm -f $(OBJS)
	make clean -C ./libft/

fclean: clean
	rm -f $(NAME)
	make fclean -C ./libft/


re: fclean all

.PHONY: all clean fclean re
