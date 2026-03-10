#include <stdio.h>
#include <ctype.h>
#include "TQueue.h"
#include "main_support.h"

#define MAX 1024

int main(int argc, char* argv[])
{
	struct TQueue queue1;
	struct TQueue queue2;

	queue_init(&queue1);
	queue_init(&queue2);

	char c;
	int len = 0;
	bool palindrom = true;

	while (1) {
		c = getchar();

		bool should_process = (c == ' ' || c == '\n');

		if (should_process && len > 0) {
			printf("Slovo: '");
			for (struct TQueueIterator it = queue_iterator_begin(&queue1); queue_iterator_is_valid(&it); queue_iterator_to_next(&it))
				printf(TQUEUE_ELEMENT_FRMSTR, queue_iterator_value(&it));
			printf("' ");

			for (size_t i = 0; i < len; i++) {
				TQueueElement queueChar1;
				TQueueElement queueChar2;
				UNLESS(queue_front(&queue1, &queueChar1), goto clean_up);
				UNLESS(queue_front(&queue2, &queueChar2), goto clean_up);
				if (queueChar1 != queueChar2) {
					palindrom = false;
				}
				UNLESS(queue_pop(&queue1), goto clean_up);
				UNLESS(queue_pop(&queue2), goto clean_up);
			}

			printf("%s\n", palindrom ? "je palindrom." : "neni palindrom.");
			palindrom = true;
			len = 0;
		}

		if (c == '\n')
			break;

		if (c != ' ') {
			UNLESS(queue_push(&queue1, c), goto clean_up);
			UNLESS(queue_push_front(&queue2, c), goto clean_up);
			len++;
		}
	}

clean_up:
	queue_destroy(&queue1);
	queue_destroy(&queue2);
	return 0;
}

