# Compiler and flags
CC=gcc
CFLAGS=-Wall -g

# Target executable name
TARGET=ourpipe

# Source files
SRCS=ourpipe.c filepipe.c truepipe.c socketpipe.c

# Object files
OBJS=$(SRCS:.c=.o)

# Header files
HDRS=ourpipe.h filepipe.h truepipe.h socketpipe.h

# Default target
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

# Include the dependencies
-include $(SRCS:.c=.d)

# Rule to compile source files and generate dependency files
%.o: %.c
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

# Clean up
clean:
	rm -f $(OBJS) $(TARGET) $(SRCS:.c=.d)
	rm -f pipe_*

