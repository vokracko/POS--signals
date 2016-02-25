CC=gcc
CFLAGS=-Wall -Wextra -pedantic

all:
	$(CC) $(CFLAGS) proj1.c -o proj1 -lpthread

clean:
	rm -f proj1

