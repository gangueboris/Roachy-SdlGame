main.exe: function.o main.o
	gcc function.o main.o -o roach -lm -lSDL
function.o: function.c roach.h
	gcc -g -c function.c -o function.o -lm
main.o: main.c roach.h
	gcc -g -c main.c -o main.o
clean:
	rm *.o roach