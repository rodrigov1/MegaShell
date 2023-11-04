# Compiler and flags
CC = gcc
CFLAGS = -Wall -pedantic -Werror

# Source files
SRC = main.c manage_commands.c cd.c clr.c quit.c

# Object files (automatically generated from SRC)
OBJ = $(SRC:.c=.o)

# Target executable
TARGET = myshell

# Get the current date and time
DATE := $(shell date)

# Include directory
INCDIR = /headers

# Build the target executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)	-I$(INCDIR)

# Build individual object files
%.o: %.c
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

# Clean rule to remove object files and the executable
clean:
	rm -f $(OBJ) $(TARGET)