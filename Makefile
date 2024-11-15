# Name of the executable
NAME = minishell

READLINE = readline
# Compiler
CC = cc

# Compiler flags
CFLAGS = -Wall -Wextra -Werror -I. -I./$(LIBS_DIR)/$(READLINE)/include #-fsanitize=address -g3

# Libraries to link with
LIBS_DIR = libraries
READLINE_LIB_PATH = $(LIBS_DIR)/readline/lib

# Source files
SRCS = minishell.c \
		tokenization.c \
		lst_utils.c \
		utils.c \
		built_in.c \
		ft_split.c \
		echo.c \
		cd.c \
		pwd_env_unset.c \
		exit.c \
		ft_atoi.c \
		export.c \
		merge_sort.c \
		free.c \
		here_doc.c \
		pipex.c \
		redirs.c \
		signals.c \
		syntax.c \
		ft_itoa.c
		
# Object files
OBJS = $(SRCS:.c=.o)

# Default rule to build the target
all: $(READLINE_LIB_PATH) $(NAME)

# Rule to link the executable
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBS) -l$(READLINE) -L$(READLINE_LIB_PATH)

# Rule to compile source files into object files
%.o: %.c Makefile minishell.h
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to build Readline
$(READLINE_LIB_PATH):
	./$(LIBS_DIR)/config_readline readline

clean:
	rm -f $(NAME) $(OBJS)

# Clean up generated files
fclean: clean
	rm -rf $(LIBS_DIR)/$(READLINE)
	make clean -C $(LIBS_DIR)/readline-8.2

# Full clean and rebuild
re: fclean all

# Phony targets
.PHONY: all fclean clean re