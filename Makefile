# Name of the executable
NAME = minishell

READLINE = readline
# Compiler
CC = cc

# Compiler flags
CFLAGS = -Wall -Wextra -Werror -I. -I./$(LIBS_DIR)/$(READLINE)/include -fsanitize=address -g3

# Libraries to link with
LIBS_DIR = libraries
READLINE_LIB_PATH = $(LIBS_DIR)/readline/lib

# Source files
SRCS =	minishell.c \
		env_copy.c \
		tokenization.c \
		tokenization_quote_1.c \
		tokenization_quote_2.c \
		tokenization_dollar.c \
		tokenization_remove.c \
		tokenization_env.c \
		tokenization_operator.c \
		utils_1.c \
		utils_2.c \
		utils_3.c \
		built_in.c \
		matrix.c \
		ft_split.c \
		echo.c \
		cd.c \
		pwd_env.c \
		unset.c \
		exit.c \
		ft_atoi.c \
		export.c \
		export1.c \
		export2.c \
		export_helpers.c \
		merge_sort.c \
		free.c \
		here_doc.c \
		pipex.c \
		redirs.c \
		signals.c \
		syntax.c \
		signals_helper.c \
		ft_itoa.c \
		cmd_1.c \
		cmd_2.c \
		fork.c \
		open_check_file.c
		
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