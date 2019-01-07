

SRC=$(wildcard ../src*.c)

CFLAGS=-g

all:$(patsubst %.c,%.out,$(wildcard *.c))

%.out:$(patsubst %.c,%.c.o,$(SRC))
	gcc $(CFLAGS) -o $@ $^

%.c.o:%.c
	gcc $(CFLAGS) -c -o $@ $^

clean:
	rm -rf $(patsubst %.c,%.out,$(wildcard *.c))
	rm -rf $(patsubst %.c,%.c.o,$(wildcard *.c))
	rm -rf $(patsubst %.c,%.c.o,$(SRC))

	