#==============================================================================#
#                              COMPILER & FLAGS                                #
#==============================================================================#

CC			=	cc
CFLAGS		=	-Wall -Wextra -Werror
CFLAGS += -fsanitize=thread

#==============================================================================#
#                                     NAMES                                    #
#==============================================================================#

NAME		=	philo

#==============================================================================#
#                              SOURCE & OBJECTS                                #
#==============================================================================#

SRC			=	main.c init.c table.c actions.c checks.c utilities.c
OBJ			=	$(SRC:.c=.o)
HFILE		=	philo.h

#==============================================================================#
#                               COLORS & EMOJIS                                #
#==============================================================================#

RESET		=	\033[0m
RED			=	\033[31m
GREEN		=	\033[32m
YELLOW		=	\033[33m
BLUE		=	\033[34m
MAGENTA 	=	\033[35m
CYAN		=	\033[36m
BOLD		=	\033[1m

SPARKLES	= ✨
HAMMER		= 🔨
CHECKMARK	= ✅
TRASH		= 🗑️
BUG			= 🐞
INFO		= ℹ️

#==============================================================================#
#                                  MESSAGES                                    #
#==============================================================================#

#INFO		=	$(CYAN)INFO$(RESET)
#SUCCESS		=	$(GREEN)SUCCESS$(RESET)
#ERROR		=	$(RED)ERROR$(RESET)

#==============================================================================#
#                                    RULES                                     #
#==============================================================================#

all: $(NAME)

$(NAME): $(OBJ)
	@echo -e "$(INFO): Compiling $(BOLD)$(NAME)$(RESET)..."
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ)
	@echo -e "$(CHECKMARK) $(GREEN)$(BOLD)$(NAME) compiled successfully!$(RESET)"

debug: CFLAGS += -g	

debug: re
	@echo -e "$(HAMMER) $(YELLOW)Compiling $(BOLD)$<$(RESET)..."

%.o: %.cc
	@echo -e "$(HAMMER) $(YELLOW)Compiling $(BOLD)$<$(RESET)..."
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo -e "$(TRASH) $(RED)Cleaning object files...$(RESET)"
	@rm -f $(OBJ)
	@echo -e "$(CHECKMARK) $(GREEN)Object files cleaned!$(RESET)"

fclean: clean
	@echo -e "$(TRASH) $(RED)Cleaning executables...$(RESET)"
	@rm -f $(NAME)
	@echo -e "$(CHECKMARK) $(GREEN)Executable cleaned!$(RESET)"

re: fclean all

.PHONY: all clean fclean re debug
