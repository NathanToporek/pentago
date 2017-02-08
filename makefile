exe: testCode.o boardstate.o
	gcc -o exe testCode.o boardstate.o

testCode.o: testCode.c boardstate.h
	gcc -c testCode.c

boardstate.o: boardstate.c boardstate.h
	gcc -c boardstate.c
