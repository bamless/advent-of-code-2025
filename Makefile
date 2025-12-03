CFLAGS=-std=c11 -Wall -Wextra

day1/day1: day1/day1.c build
	$(CC) $(CFLAGS) ./day1/day1.c -o ./build/day1

build:
	mkdir -p build
