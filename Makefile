##
# Esh
#
# @file
# @version 0.1

CC = gcc
CFLAGS = -Wall -I./include
SRCS = $(wildcard src/*.c)
OBJS = $(SRCS:.c=.o)
TARGET = esh

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

# end
