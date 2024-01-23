# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lseiberr <lseiberr@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/22 11:56:27 by lseiberr          #+#    #+#              #
#    Updated: 2024/01/23 09:45:13 by lseiberr         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= ircserver

CC		= g++
CFLAGS	= -Werror -Wextra -Wall -std=c++98

SRC		= src/main.cpp src/server/server.cpp
OBJ		= $(SRC:.cpp=.o)

#Colors

DEF_COLOR = \033[0;39m
GRAY = \033[0;90m
RED = \033[0;91m
GREEN = \033[0;92m
YELLOW = \033[0;93m
BLUE = \033[0;94m
MAGENTA = \033[0;95m
CYAN = \033[0;96m
WHITE = \033[0;97m

#Message

COMP_START      =       @printf "\n$(BOLD_YELLOW)Make: $(NO_COLOR)$(BOLD)Debut de compilation...\r$(NO_COLOR)"

EXE_READY       =       @echo "\n\n$(BOLD)Compilation de $(BOLD_PURPLE)$(NAME)$(NO_COLOR) $(BOLD)reussi !$(NO_COLOR)\n"

CLEANED         =       @echo "\n$(BOLD_YELLOW)Clean: $(NO_COLOR)Suppression des fichiers .o et de l'executable \n"

FCLEANED        =       @echo "\n$(BOLD_YELLOW)Fclean: $(NO_COLOR)Suppression des fichiers .o et de l'executable \n"

all: $(NAME)

comp_start:
	$(COMP_START)

$(NAME): comp_start $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@
	$(EXE_READY)

%.o: %.cpp
	@printf "$(BOLD_YELLOW)Make: $(NO_COLOR)$(BOLD)Compilation des fichiers :$(BOLD_CYAN) %-33.33s $(BOLD_YELLOW)[%3d%%] \r$(NO_COLOR)" $? $
	@$(CC) $(CFLAGS) -c $< -o $@
	@printf "$(BOLD_YELLOW)Make: $(NO_COLOR)$(BOLD)Compilation des fichiers :$(BOLD_CYAN) %-33.33s $(BOLD_YELLOW)[%3d%%] \r$(NO_COLOR)" $? $

clean:
	@rm -f *.o
	@rm -f src/*.o
	@rm -f src/server/*.o
	@rm -f src/client/*.o
	$(CLEANED)

fclean: clean
	@rm -f $(NAME)
	$(FCLEANED)

re: fclean all

.PHONY: all re clean fclean
