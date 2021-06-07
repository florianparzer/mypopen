#Options
#-Wall		Turns on most compiler warnings
#-Wextra	Turns on more compiler warnings
#-pedantic	Turns on even more compiler warnings
#-Werror	Treats compiler warnings as errors

OPTIONS = -Wall -Wextra -pedantic
all: mypopen

mypopen: main.o mypopen.o
	gcc $(OPTIONS) main.o mypopen.o -o mypopen

main.o: main.c
	gcc $(OPTIONS) -c main.c

mypopen.o: mypopen.c mypopen.h
	gcc $(OPTIONS) -c mypopen.c

clean:
	rm -f mypopen
	rm -f *.o
