# Name of the executable
TARGET = minishell

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -Werror -I. -g3 -fsanitize=address

# Libraries to link with
LIBS = -lreadline -lncurses

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
		pipex.c
		
# Object files
OBJS = $(SRCS:.c=.o)

# Default rule to build the target
all: $(TARGET)

# Rule to link the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

# Rule to compile source files into object files
%.o: %.c Makefile minishell.h
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up generated files
fclean:
	rm -f $(TARGET) $(OBJS)

re: fclean all

# Phony targets
.PHONY: all clean
