# simple compilation: 
# gcc driver.c queue.c hashmaps.c -o driver.x
CC = gcc
CFLAGS = -ansi -pedantic-errors -Wall -Werror -Wshadow -fstack-protector-all
HDRS = driver.h queue.h hashmaps.h
OBJS = driver.o queue.o hashmaps.o
FLS = driver.c queue.c hashmaps.c

all: driver.x

driver.x: $(FLS) $(HDRS)
	$(CC) $(CFLAGS) $(FLS) -o driver.x
test:
	driver.x < test.txt
	
clean:
	rm -rf driver.x *.o
