exe: driver.o boardstate.o
	gcc -o exe driver.o boardstate.o

driver.o: driver.c boardstate.h
	gcc -c driver.c

boardstate.o: boardstate.c boardstate.h
	gcc -c boardstate.c
