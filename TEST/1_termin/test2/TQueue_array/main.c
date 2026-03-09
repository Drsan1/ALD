#include <stdio.h>
#include <ctype.h>
#include "TStack.h"
#include "TQueue.h"
#include "main_support.h"

#define MAX 1024

int main(int argc, char* argv[])
{
	struct TStack stack1;
	struct TQueue queue1;

	stack_init(&stack1);
	queue_init(&queue1);
	
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
				TStackElement stackChar;
				TQueueElement queueChar;
				UNLESS(stack_top(&stack1, &stackChar), goto clean_up);
				UNLESS(queue_front(&queue1, &queueChar), goto clean_up);
				if (stackChar != queueChar) {
					palindrom = false;
				}
				UNLESS(stack_pop(&stack1), goto clean_up);
				UNLESS(queue_pop(&queue1), goto clean_up);
			}

			printf("%s\n", palindrom ? "je palindrom." : "neni palindrom.");
			palindrom = true;
			len = 0;
		}

		if (c == '\n')
			break;

		if (c != ' ') {
			UNLESS(stack_push(&stack1, c), goto clean_up);
			UNLESS(queue_push(&queue1, c), goto clean_up);
			len++;
		}
	}

clean_up:
	stack_destroy(&stack1);
	queue_destroy(&queue1);
	return 0;
}

