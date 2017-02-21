exe.exe: driver.o boardstate.o
	gcc -o exe.exe driver.o boardstate.o

test.exe: testCode.o linkedlist.o boardstate.o gametree.o
	gcc -o test.exe testCode.o linkedlist.o boardstate.o gametree.o

clean:
	rm *.o *.exe

testCode.o: testCode.c boardstate.h gametree.h
	gcc -c testCode.c

driver.o: driver.c boardstate.h
	gcc -c driver.c

boardstate.o: boardstate.c boardstate.h
	gcc -c boardstate.c

gametree.o: gametree.c gametree.h
	gcc -c gametree.c
	
linkedlist.o: linkedlist.c linkedlist.h
	gcc -c linkedlist.c
