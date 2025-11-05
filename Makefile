# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -O2
LDFLAGS = -lm

# Target executable name
TARGET = tsp

# Default rule
all: $(TARGET)

# Build rule
$(TARGET): tsp.c
	$(CC) $(CFLAGS) tsp.c -o $(TARGET) $(LDFLAGS)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Clean up build artifacts
clean:
	rm -f $(TARGET)
