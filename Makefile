# Simple makefile for kakfa-c-trivial

kafka-c-trivial: main.c
	$(CC) -Wall -Wextra -o kafka-c-trivial main.c -lrdkafka

clean:
	rm -f kafka-c-trivial

