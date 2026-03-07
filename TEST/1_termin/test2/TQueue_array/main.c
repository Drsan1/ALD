#include <stdio.h>
#include <ctype.h>
#include "TStack.h"
#include "TQueue.h"
#include "main_support.h"

#define MAX 1024

int main(int argc, char* argv[])
{
	char buffer[MAX];
	struct TStack stack1;
	struct TQueue queue1;

	stack_init(&stack1);
	queue_init(&queue1);

	size_t count = 0;
	while (count < MAX - 1 && (buffer[count] = getchar()) != '\n') {
		count++;
	}
	buffer[count] = ' ';

	size_t len = 0;
	bool palindrom = true;
	char slovo[MAX];

	for (size_t i = 0; i < count + 1; i++) {
		if (buffer[i] == ' ') {
			if (len > 0) {
				slovo[len] = '\0';
				for (size_t j = 0; j < len; j++) {
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
				if (printf("Slovo: '%s' %s\n", slovo, palindrom ? "je palindrom." : "neni palindrom.") < 0)
					return 5;
				palindrom = true;
				len = 0;
			}
		}
		else {
			slovo[len] = buffer[i];
			stack_push(&stack1, buffer[i]);
			queue_push(&queue1, buffer[i]);
			len++;
		}
	}

clean_up:
	stack_destroy(&stack1);
	queue_destroy(&queue1);
	return 0;
}



