# simple compilation: 
# gcc driver.c queue.c hashmaps.c -o driver.x
CC = gcc
CFLAGS = -ansi -pedantic-errors -Wall -Werror -Wshadow -fstack-protector-all
HDRS = driver.h queue.h hashmaps.h
OBJS = driver.o queue.o hashmaps.o
FLS = driver.c queue.c hashmaps.c

all: driver.x

driver.x: driver.o queue.o hashmaps.o
	$(CC) $(OBJS) -o driver.x

driver.o: driver.c driver.h hashmaps.h
	$(CC) $(CFLAGS) -c driver.c

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
