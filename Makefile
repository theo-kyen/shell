OBJ = cmd.o

%.o: %.c
	gcc -c -o $@ $< -g

make: $(OBJ)
	gcc main.c -o main.exe $^ -g

clean:
	rm main.exe
	rm *.o
