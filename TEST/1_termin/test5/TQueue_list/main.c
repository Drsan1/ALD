#include <stdio.h>
#include "TQueue.h"
#include "main_support.h"

int main(int argc, char *argv[])
	{
	struct TQueue queue1 = { 0 };
	queue_init(&queue1);

	while (true) {
		int new;
		if (scanf("%d", &new) != 1) {
			return 1;
		}
		if (new < 0)
			break;
		if (!queue_push(&queue1, new))
			return 2;
	}
	printf("queue1: ");
	for (struct TQueueIterator it = queue_iterator_begin(&queue1); queue_iterator_is_valid(&it); queue_iterator_to_next(&it))
		printf(TQUEUE_ELEMENT_FRMSTR " ", queue_iterator_value(&it));
	printf("\n");

	struct TQueue queue2 = { 0 };
	if (!queue_clone(&queue1, &queue2))
		return 3;

	queue_destroy(&queue1);
	printf("queue1 is empty: %s\n", bool_to_cstring(queue_is_empty(&queue1)));
	
	printf("queue2: ");
	for (struct TQueueIterator it = queue_iterator_begin(&queue2); queue_iterator_is_valid(&it); queue_iterator_to_next(&it))
		printf(TQUEUE_ELEMENT_FRMSTR " ", queue_iterator_value(&it));
	printf("\n");
	queue_destroy(&queue2);
	return 0;
	}

/** \} MainProgram */
