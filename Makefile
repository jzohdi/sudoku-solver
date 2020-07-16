# simple compilation:
# gcc sudoku.c queue.c hashmaps.c -o driver.x
CC = gcc
CFLAGS = -ansi -pedantic-errors -Wall -Werror -Wshadow -fstack-protector-all
HDRS = sudoku.h queue.h hashmaps.h
OBJS = sudoku.o queue.o hashmaps.o
FLS = sudoku.c queue.c hashmaps.c

all: sudoku.x

manual:
	gcc sudoku.c queue.c hashmaps.c -o sudoku.x

sudoku.x: sudoku.o queue.o hashmaps.o
	$(CC) $(OBJS) -o sudoku.x

sudoku.o: sudoku.c sudoku.h hashmaps.h
	$(CC) $(CFLAGS) -c sudoku.c

queue.o: queue.c queue.h
	$(CC) $(CFLAGS) -c queue.c

hashmaps.o: hashmaps.c hashmaps.h
	$(CC) $(CFLAGS) -c hashmaps.c

test:
	driver.x < test01.input
	driver.x < test02.input
	driver.x < test03.input

testdiff:
	driver.x < test01.input | diff - test01.output
	driver.x < test02.input | diff - test02.output
	driver.x < test03.input | diff - test03.output

clean:
	rm -rf driver.x *.o
