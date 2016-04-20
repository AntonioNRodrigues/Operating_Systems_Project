#include <assert.h>
#include <stdio.h>

#include "fifo.h"

void fifoInit (FIFO *fifo)
{
	fifo->pin = 0;
	fifo->pout = 0;
	fifo->size = 0;
}

void fifoPush (FIFO *fifo, int element)
{
	assert (fifo->size < CAPACITY);
	fifo->data [fifo->pin] = element;
	fifo->pin = (fifo->pin + 1) % CAPACITY;
	fifo->size++;
}

int fifoPop (FIFO *fifo)
{
	assert (fifo->size > 0);
	int result = fifo->data [fifo->pout];
	fifo->pout = (fifo->pout + 1) % CAPACITY;
	fifo->size--;
	return result;
}

void fifoPrint (FIFO *fifo)
{
	if (fifo->size > 0) {
		printf ("%d", fifo->data [fifo->pout]);
		int i = fifo->pout;
		i = (i + 1) % CAPACITY;
		while (i != fifo->pin) {
			printf (" %d", fifo->data [i]);
			i = (i + 1) % CAPACITY;
		}
	}
}
