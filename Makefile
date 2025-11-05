# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -O2

# Target executable name
TARGET = tsp

# Default rule
all: $(TARGET)

# Build rule
$(TARGET): tsp.c
	$(CC) $(CFLAGS) tsp.c -o $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Clean up build artifacts
clean:
	rm -f $(TARGET)
