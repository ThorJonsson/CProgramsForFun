# The compiler
CC = gcc
#  Compiler flags
#  -Wall turns on most, but not all, compiler warnings
CFLAGS = -std=c99 -Wall -Wextra -g -O0
# preprocessor flags - This is for compiling
CPPFLAGS = -I.
# linking flags - This is for linking
LDFLAGS = -L.

# rm command
RM = rm -f  

# the build target executable
TARGET = colored_ropes

.PHONY: all clean


all: $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c
	
clean:
	${RM} $(TARGET) *.o *~