CC := gcc
CFLAGS := -lSDL3 -Wall -Wextra -Werror

emulator:
	$(CC) emulator.c -o run $(CFLAGS)