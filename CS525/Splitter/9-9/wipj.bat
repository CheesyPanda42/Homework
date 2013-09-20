
	rm b.exe
	rm wipj.o
	gcc -c wipj.c -Wall -Wextra -ansi -pedantic
	gcc -o b.exe wipj.o -Wall -Wextra -ansi -pedantic