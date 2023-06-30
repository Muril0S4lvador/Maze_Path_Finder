all: 
	gcc -g -o main *.c

clean:
	rm -r main

do:
	./main < xtestrdeque/in/1 > xtestrdeque/my/my1
	./main < xtestrdeque/in/2 > xtestrdeque/my/my2
	./main < xtestrdeque/in/3 > xtestrdeque/my/my3
	./main < xtestrdeque/in/4 > xtestrdeque/my/my4
	./main < xtestrdeque/in/5 > xtestrdeque/my/my5
	./main < xtestrdeque/in/6 > xtestrdeque/my/my6
	./main < xtestrdeque/in/7 > xtestrdeque/my/my7
	./main < xtestrdeque/in/8 > xtestrdeque/my/my8
	./main < xtestrdeque/in/9 > xtestrdeque/my/my9
	./main < xtestrdeque/in/10 > xtestrdeque/my/my10
	./main < xtestrdeque/in/11 > xtestrdeque/my/my11
	./main < xtestrdeque/in/12 > xtestrdeque/my/my12

compare:
	diff xtestrdeque/out/1 xtestrdeque/my/my1
	diff xtestrdeque/out/2 xtestrdeque/my/my2
	diff xtestrdeque/out/3 xtestrdeque/my/my3
	diff xtestrdeque/out/4 xtestrdeque/my/my4
	diff xtestrdeque/out/5 xtestrdeque/my/my5
	diff xtestrdeque/out/6 xtestrdeque/my/my6
	diff xtestrdeque/out/7 xtestrdeque/my/my7
	diff xtestrdeque/out/8 xtestrdeque/my/my8
	diff xtestrdeque/out/9 xtestrdeque/my/my9
	diff xtestrdeque/out/10 xtestrdeque/my/my10
	diff xtestrdeque/out/11 xtestrdeque/my/my11
	diff xtestrdeque/out/12 xtestrdeque/my/my12