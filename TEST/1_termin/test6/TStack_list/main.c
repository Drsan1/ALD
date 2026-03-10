#include <stdio.h>
#include <ctype.h>
#include "TStack.h"
#include "main_support.h"

int main(int argc, char *argv[])
	{
	if (argc != 2)
		return 5;
	struct TStack stack = { 0 };
	FILE* fr = fopen(argv[1], "r");
	if (!fr)
		return 1;
	if (!stack_init_file(&stack, fr))
		return 2;

	for(struct TStackIterator it = stack_iterator_begin(&stack); stack_iterator_is_valid(&it); stack_iterator_to_next(&it))
		printf(TSTACK_ELEMENT_FRMSTR " ", stack_iterator_value(&it));
	putchar('\n');

	TStackElement val;
	if (!stack_is_empty(&stack)) {
		UNLESS(stack_top(&stack, &val), goto clean_up);
		UNLESS(printf("prvni: " TSTACK_ELEMENT_FRMSTR "\n", val), goto clean_up);
		UNLESS(stack_pop(&stack), goto clean_up);
	}

	while (!stack_is_empty(&stack)) {
		TStackElement val12;
		if (!stack_is_empty(&stack)) {
			UNLESS(stack_top(&stack, &val12), goto clean_up);
			if(val12 < val)
				UNLESS(printf("\n"), goto clean_up);
		}
		UNLESS(stack_pop(&stack), goto clean_up);
		UNLESS(printf(TSTACK_ELEMENT_FRMSTR "\n", val12), goto clean_up);
		val = val12;
	}

	stack_destroy(&stack);
	fclose(fr);

clean_up:
	return 0;
	}

/** \} MainProgram */
