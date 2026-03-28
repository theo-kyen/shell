OBJ = cmd.o

%.o: %.c
	gcc -c -o $@ $< -g

make: $(OBJ)
	gcc main.c -o main.exe $^ -g

lex:
	yacc -d parser.y
	lex scanner.l
	cc lex.yy.c -ll
	gcc lex.yy.c y.tab.c -o lex.exe

clean:
	rm -f y.tab.*
	rm -f lex.yy.c
	rm -f *.exe
	rm -f *.o
	rm -f *.out
