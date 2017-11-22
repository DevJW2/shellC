all: shell.c
	gcc -o shelling shell.c

run: all
	./shelling

clean:
	rm shelling
	rm *.o
	rm *~

