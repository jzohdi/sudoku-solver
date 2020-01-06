all: driver.x

driver.x: driver.c driver.h queue.c queue.h hashmaps.h
	gcc driver.c queue.c -o driver.x

test:
	driver.x < test.txt
	
clean:
	rm -rf driver.x