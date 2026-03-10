/** \file TStack.c
 *  \brief Implementace API pro typ zásobník (realizace pomocí lineárního jednosměrně vázaného seznamu)
 *  \author Petyovský
 *  \version 2026
 *  $Id: TStack.c 3201 2026-02-13 18:07:58Z petyovsky $
 */

#include "TStack.h"

/** \brief Úplná definice privátního typu StackNode
 *  \details Privátní typ StackNode (nedostupný mimo funkce ze souboru TStack.c) reprezentuje typ pro jednotlivé uzly lineárního jednosměrně vázaného seznamu.
 */
struct TStackNode
	{
	struct TStackNode *iNext;				///< Ukazatel na následující uzel lineárního jednosměrně vázaného seznamu
	TStackElement iValue;					///< Element zásobníku uložený v uzlu lineárního jednosměrně vázaného seznamu
	};

void stack_init(struct TStack *aStack)
	{
	if (!aStack)
		return;
	aStack->iTop = NULL;
	}

bool stack_is_empty(const struct TStack *aStack)
	{
	if (!aStack)
		return true;
	return aStack->iTop == NULL;
	}

bool /* TStackIterator */ stack_top(const struct TStack *aStack, TStackElement *aValue)
	{
	if (stack_is_empty(aStack) || !aValue)
		return false;
	*aValue = aStack->iTop->iValue;
	return true;
	}

bool stack_push(struct TStack *aStack, TStackElement aValue)
	{
	if (!aStack)
		return false;
	struct TStackNode* ptr = malloc(sizeof(struct TStackNode));
	if (!ptr)
		return false;
	*ptr = (struct TStackNode){ .iNext = aStack->iTop, .iValue = aValue };
	aStack->iTop = ptr;
	return true;
	}

bool stack_pop(struct TStack *aStack)
	{
	if (stack_is_empty(aStack))
		return false;
	struct TStackNode* ptr = aStack->iTop;
	aStack->iTop = aStack->iTop->iNext;
	free(ptr);
	return true;
	}

void stack_destroy(struct TStack *aStack)
	{
	if (!aStack)
		return;
	while (!stack_is_empty(aStack))
		stack_pop(aStack);
	}

bool stack_init_file(struct TStack* aStack, FILE* aInputfile) {
	if (!aStack || !aInputfile)
		return false;
	int count;
	if (fscanf(aInputfile, "%d", &count) != 1)
		return false;
	stack_init(aStack);
	for (size_t i = 0; i < count; i++) {
		TStackElement tmp;
		if (fscanf(aInputfile, TSTACK_ELEMENT_FRMSTR, &tmp) != 1)
			return false;

		struct TStackNode* ptr = malloc(sizeof(struct TStackNode));
		if (!ptr) {
			stack_destroy(aStack);
			return false;
		}
		*ptr = (struct TStackNode){ .iNext = aStack->iTop, .iValue = tmp };
		aStack->iTop = ptr;
	}
	return true;
}

struct TStackIterator stack_iterator_begin(const struct TStack* aStack)
{
	if (!stack_is_empty(aStack))
		return (struct TStackIterator) { .iStack = aStack, .iActual = aStack->iTop };
	return (struct TStackIterator) { .iStack = NULL, .iActual = NULL };
}

bool stack_iterator_is_valid(const struct TStackIterator *aIter)
	{
	if (aIter)
		if (aIter->iStack && aIter->iActual)
			return true;
	return false;
	}

bool stack_iterator_to_next(struct TStackIterator *aIter)
	{
	if (!stack_iterator_is_valid(aIter))
		return false;
	aIter->iActual = aIter->iActual->iNext;
	return aIter->iActual != NULL;
	}

TStackElement stack_iterator_value(const struct TStackIterator *aIter)
	{
	if (stack_iterator_is_valid(aIter))
		return aIter->iActual->iValue;
	return (TStackElement) { 0 };
	}
