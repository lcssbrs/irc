NAME	= ircserver

CC		= c++

FLAGS	= -Werror -Wextra -Wall -std=c++98

SRC		= src/main.cpp src/server/server.cpp src/client/client.cpp src/channel/channel.cpp

OBJS		= $(SRC:%.cpp=%.o)

# ------------------------------ Couleurs ------------------------------

BOLD_BLACK	=	\033[1;30m

BOLD_WHITE	=	\033[1;37m

BOLD_RED	=	\033[1;31m

BOLD_PURPLE	=	\033[1;35m

BOLD_CYAN	=	\033[1;36m

BOLD_GREEN	=	\033[1;32m

BOLD_YELLOW	=	\033[1;33m

BOLD_BLUE	=	\033[1;34m

RESET	=	\033[0m

# ------------------------------ Messages ------------------------------

COMP_START	=	echo "\n💻 $(BOLD_PURPLE)$(NAME) $(BOLD_WHITE)by $(BOLD_BLUE)hbaduel$(BOLD_WHITE), $(BOLD_RED)lseiberr$(BOLD_WHITE), $(BOLD_YELLOW)aandrieux $(BOLD_WHITE)& $(BOLD_GREEN)tde-los\n"

EXE_READY	=	echo "\n\n💬 $(BOLD_WHITE)Compilation de $(BOLD_BLUE)$(NAME)$(BOLD_GREEN) réussie !\n"

CLEANED		=	echo "\n🗑️  $(BOLD_WHITE)Suppression des objets...\n"

FCLEANED	=	echo "\n🗑️  $(BOLD_WHITE)Suppression des objets et de l'exécutable...\n"

# ------------------------------ Regles ------------------------------

MAKEFLAGS += --silent

all: $(NAME)

%.o: %.cpp
	@printf "$(RESET)🚧 $(BOLD_WHITE)Compilation des fichiers :$(BOLD_PURPLE) %-33.33s\r" $@
	@$(CC) $(FLAGS) -c $< -o $@

$(NAME): comp_lib $(OBJS)
	@$(CC) $(FLAGS) -o $(NAME) $(OBJS)
	@$(EXE_READY)

comp_lib :
	@$(COMP_START)

clean:
	$(CLEANED)
	rm -rf $(OBJS)

fclean:
	$(FCLEANED)
	rm -rf $(NAME) $(OBJS)

re: fclean all

.phony: all clean fclean
