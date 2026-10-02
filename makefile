CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

TARGETS = bot.exe test.exe

BOT_OBJS = src/bot.o src/screenshot.o src/tango-rules.o
TEST_OBJS = src/test.o src/tango-rules.o

.PHONY: all default clean

default: all

all: $(TARGETS)

bot.exe: $(BOT_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

test.exe: $(TEST_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del /Q src\*.o $(TARGETS)