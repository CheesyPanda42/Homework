	rm test*
	rm a.exe
	rm wip.o
	gcc -c wip.c -Wall -Wextra -ansi -pedantic
	gcc -o a.exe wip.o -Wall -Wextra -ansi -pedantic