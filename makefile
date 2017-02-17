exe: driver.o boardstate.o
	gcc -o exe driver.o boardstate.o

test: testCode.o boardstate.o
	gcc -o test testCode.o boardstate.o

clean:
	rm *.o exe test
    
testCode.o: testCode.c
	gcc -c testCode.c

driver.o: driver.c boardstate.h
	gcc -c driver.c

boardstate.o: boardstate.c boardstate.h
	gcc -c boardstate.c

