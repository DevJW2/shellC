shelling: shell.o main.o
	gcc -o shelling shell.o main.o
shell.o: shell.c
	gcc -c shell.c
main.o: main.c
	gcc -c main.c
run: shelling
	./shelling
clean:
	rm *.o



