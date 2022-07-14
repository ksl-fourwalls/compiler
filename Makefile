all:
	gcc *.c -o compiler
run:
	./compiler
debug:
	gcc *.c -g -o compiler
	gdb ./compiler
clean:
	rm compiler
