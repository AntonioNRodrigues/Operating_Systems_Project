#ifndef __FIFO__
#define __FIFO__

#define CAPACITY 100

typedef struct {
	int data [CAPACITY];
	int pin;
	int pout;
	int size;
} FIFO;

void fifoInit (FIFO *fifo);

void fifoPush (FIFO *fifo, int element);

int fifoPop (FIFO *fifo);

void fifoPrint (FIFO *fifo);

#endif
