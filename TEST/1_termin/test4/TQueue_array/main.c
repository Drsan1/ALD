#include <stdio.h>
#include <math.h>
#include "TQueue.h"
#include "main_support.h"
#define MAX 1024

int main(int argc, char *argv[])
	{
	struct TQueue queue;
	char buffer[MAX];
	size_t count = 0;
	while (count < MAX - 1 && (buffer[count] = getchar()) != '\n') {
		count++;
	}
	queue_init(&queue);
	for (size_t i = 0; i < count; i++) {
		if (!queue_push(&queue, buffer[i]))
			return 1;
	}
	int num;
	if (scanf("%d", &num) != 1)
		return 2;
	for (struct TQueueIterator it = queue_iterator_begin(&queue); queue_iterator_is_valid(&it); queue_iterator_to_next(&it))
		printf(TQUEUE_ELEMENT_FRMSTR "", queue_iterator_value(&it));
	putchar('\n');
	for (size_t i = 0; i < abs(num); i++) {
		if (num > 0) {
			TQueueElement temp;
			queue_front(&queue, &temp);
			queue_pop(&queue);
			queue_push(&queue, temp);
		}
		else {
			TQueueElement temp;
			queue_back(&queue, &temp);
			queue_pop_back(&queue);
			queue_push_front(&queue, temp);
		}
		for (struct TQueueIterator it = queue_iterator_begin(&queue); queue_iterator_is_valid(&it); queue_iterator_to_next(&it))
			printf(TQUEUE_ELEMENT_FRMSTR "", queue_iterator_value(&it));
		putchar('\n');
	}


	queue_destroy(&queue);
	return 0;
	}

/** \} MainProgram */

