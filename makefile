# Makefile for BitBoard Checkers Project

CC = gcc
CFLAGS = -Wall -Wextra -std=c11
OBJ = main.o board.o moves.o save_load.o
TARGET = checkers

# Default rule
all: $(TARGET)

# Link object files into final program
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

# Compile each .c file into .o file
main.o: main.c board.h moves.h save_load.h
	$(CC) $(CFLAGS) -c main.c

board.o: board.c board.h
	$(CC) $(CFLAGS) -c board.c

moves.o: moves.c moves.h board.h
	$(CC) $(CFLAGS) -c moves.c

save_load.o: save_load.c save_load.h
	$(CC) $(CFLAGS) -c save_load.c

# Clean up build artifacts
clean:
	rm -f $(OBJ) $(TARGET)

# Run the program
run: all
	./$(TARGET)
