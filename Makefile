# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nmallett <nmallett@student.42quebec.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/04/28 13:21:46 by nmallett          #+#    #+#              #
#    Updated: 2022/04/28 13:52:10 by nmallett         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS = client.c server.c
OBJS = $(SRCS:.c=.o)
CC = gcc
FLAGS = -Wall -Wextra -Werror

all: client server

bonus: client server

client: client.o libft
	$(CC) -o $@ $< -Llibft -lft
	
server: server.o libft
	$(CC) -o $@ $< -Llibft -lft

%.o: %.c
	$(CC) -c $(CFLAGS) $?
	
libft :
		make -C libft

clean : 
		rm -rf $(OBJS)
		make -C libft clean

fclean : clean
		rm -rf client server
		make -C libft fclean
		
re : fclean all

.PHONY: all bonus libft clean fclean re