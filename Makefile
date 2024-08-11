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
		utils.c
		
# Object files
OBJS = $(SRCS:.c=.o)

# Default rule to build the target
all: $(TARGET)

# Rule to link the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

# Rule to compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up generated files
fclean:
	rm -f $(TARGET) $(OBJS)

re: fclean all

# Phony targets
.PHONY: all clean
