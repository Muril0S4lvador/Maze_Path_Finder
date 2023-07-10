all: 
	gcc -g -o main *.c -lm

clean:
	rm -r main

deque: deque.h
	gcc -g -o main main.c deque.c

hash: hash.h forward_list.h
	gcc -g -o main forward_list.c hash.c main.c
