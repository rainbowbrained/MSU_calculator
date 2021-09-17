CC=gcc --coverage -O2 -ftrapv -fsanitize=address,undefined -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op -Wstrict-prototypes -Wold-style-declaration -Wold-style-definition -Wmissing-parameter-type -Wmissing-field-initializers -Wnested-externs -Wno-pointer-sign -std=gnu11 -lm

main: main.o polis.o stack.o
	$(CC) -O0 -g main.o stack.o polis.o -o main

main.o: main.c
	$(CC) -O0 -g -c main.c -o main.o

stack.o: stack.c
	$(CC) -c stack.c -o stack.o

polis.o: polis.c
	$(CC) -O0 -g -c polis.c -o polis.o

clean:
	rm -rf *.o *.gcda *.gcno main
