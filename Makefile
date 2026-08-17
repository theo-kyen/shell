OBJ = cmd.o

%.o: %.c
	gcc -c -o $@ $< -g

make: $(OBJ)
	gcc main.c -o main.exe $^ -g

lex:
	yacc -v -d parser.y -Wcounterexamples
	lex scanner.l
	gcc lex.yy.c y.tab.c cmd.c -o lex.exe -g -ll

clean:
	rm -f y.tab.*
	rm -f lex.yy.c
	rm -f *.exe
	rm -f *.o
	rm -f *.out
	rm -f y.output

valgrind-%: %.exe
	valgrind --leak-check=full --show-leak-kinds=all ./$<