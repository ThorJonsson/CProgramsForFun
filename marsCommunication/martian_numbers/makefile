# The compiler
CC = gcc
#  Compiler flags
#  -Wall turns on most, but not all, compiler warnings
CFLAGS = -std=gnu99 -Wall -Wextra -g -O2

.PHONY: all clean
# the build target executable
TARGET = martian_numbers

all: $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c
	
clean:
	${RM} $(TARGET) *.o *~