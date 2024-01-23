# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lseiberr <lseiberr@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/22 11:56:27 by lseiberr          #+#    #+#              #
#    Updated: 2024/01/22 18:04:00 by lseiberr         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= ircserver

CC		= g++
CFLAGS	= -Werror -Wextra -Wall -std=c++98

SRC		= src/main.cpp src/server/server.cpp
OBJ		= $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o
	rm -f src/*.o
	rm -f src/server/*.o
	rm -f src/client/*.o

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all re clean fclean
