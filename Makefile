CC=gcc
CFLAGS=-Wall -Wextra -pedantic

all:
	$(CC) $(CFLAGS) proj01.c -o proj01 -lpthread

clean:
	rm -f proj01

