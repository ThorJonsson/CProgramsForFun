# The compiler
CC = gcc
#  Compiler flags
#  -Wall turns on most, but not all, compiler warnings
CFLAGS = -std=c99 -Wall -Wextra -g -O0

.PHONY: all clean
# the build target executable
TARGET = allpermutations

all: $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c
	
clean:
	${RM} $(TARGET) *.o *~