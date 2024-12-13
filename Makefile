# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99

# Executable name
EXEC = todolist

# Source files
SRCS = todolist.c file_handler.c

# Object files
OBJS = $(SRCS:.c=.o)

# Dependencies (headers)
DEPS = enum.h file_handler.h task.h

# Default target
all: $(EXEC)

# Link objects to create the final executable
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compile source files into object files
%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(EXEC)

# Phony targets
.PHONY: all clean
