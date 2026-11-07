CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -g
LDFLAGS = -lgdi32 -luser32

TARGETS = bot.exe test.exe sctest.exe

BOT_OBJS = src/bot.o src/screenshot.o src/tango-rules.o
TEST_OBJS = src/test.o src/tango-rules.o
SCTEST_OBJS = src/sctest.o src/screenshot.o

.PHONY: all default clean

default: all

all: $(TARGETS)

bot.exe: $(BOT_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

test.exe: $(TEST_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

sctest.exe: $(SCTEST_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del /Q src\*.o $(TARGETS)