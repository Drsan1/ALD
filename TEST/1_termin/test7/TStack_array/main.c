#include <stdio.h>
#include <ctype.h>
#include "TStack.h"
#include "main_support.h"

int main(int argc, char *argv[])
	{
	struct TStack stack = { 0 };
	stack_init(&stack);
	char c;

	while ((c = getchar()) != '\n'){
		if (isdigit(c)) {
			TStackElement value = c - '0'; // prevod char -> int
			UNLESS(stack_push(&stack, value), goto clean_up2);
		}
		if (c == '+') {
			TStackElement val1, val2;
			if (stack_is_empty(&stack)) goto clean_up1;
			UNLESS(stack_top(&stack, &val1), goto clean_up);
			UNLESS(stack_pop(&stack), goto clean_up);
			if (stack_is_empty(&stack)) goto clean_up1;
			UNLESS(stack_top(&stack, &val2), goto clean_up);
			UNLESS(stack_pop(&stack), goto clean_up);

			TStackElement result = val2 + val1;
			UNLESS(stack_push(&stack, result), goto clean_up);
		}
		if (c == '-') {
			TStackElement val1, val2;
			UNLESS(stack_top(&stack, &val1), goto clean_up);
			if (stack_is_empty(&stack)) goto clean_up1;
			UNLESS(stack_pop(&stack), goto clean_up);
			UNLESS(stack_top(&stack, &val2), goto clean_up);
			if (stack_is_empty(&stack)) goto clean_up1;
			UNLESS(stack_pop(&stack), goto clean_up);

			TStackElement result = val2 - val1;
			UNLESS(stack_push(&stack, result), goto clean_up);
		}
		if (c == '=') {
			TStackElement val;
			if (stack_is_empty(&stack)) goto clean_up1;
			UNLESS(stack_top(&stack, &val), goto clean_up);
			UNLESS(printf("result: " TSTACK_ELEMENT_FRMSTR, val), goto clean_up);
			UNLESS(stack_pop(&stack), goto clean_up);
		}
	}
	
clean_up:
	stack_destroy(&stack);
	return 0;
clean_up1:
	stack_destroy(&stack);
	printf("Chyba, prazdny zasobnik!");
	return 1;
clean_up2:
	stack_destroy(&stack);
	printf("Chyba, plny zasobnik!");
	return 2;
	
	}

/** \} MainProgram */
