/** \file TStack.c
 *  \brief Implementace API pro typ zásobník (realizace pomocí pole)
 *  \author Petyovský
 *  \version 2026
 *  $Id: TStack.c 3201 2026-02-13 18:07:58Z petyovsky $
 */

#include <assert.h>
#include "TStack.h"
#include "check.h"

[[nodiscard]] static inline bool stack_invariant(const struct TStack *aStack)
	{
	return aStack->iCount <= aStack->iCapacity;
	}

void stack_init(struct TStack *aStack)
	{
	if (!aStack)
		return;
	aStack->iCount = 0;
	aStack->iCapacity = 0;
	aStack->iValues = NULL;
	assert(stack_invariant(aStack));
	}

bool stack_is_empty(const struct TStack *aStack)
	{
	if (!aStack)
		return true;
	assert(stack_invariant(aStack));
	return aStack->iCount == 0;
	}

bool /* TStackIterator */ stack_top(const struct TStack *aStack, TStackElement *aValue)
	{
	if (stack_is_empty(aStack) || !aValue)
		return false;
	*aValue = aStack->iValues[aStack->iCount - 1];
	assert(stack_invariant(aStack));
	return true;
	}

bool stack_push(struct TStack *aStack, TStackElement aValue)
	{
	if (!aStack)
		return false;
	if (aStack->iCapacity == 0) { // Pokud nemá žádné místo 0/0
		assert(stack_invariant(aStack));
		aStack->iValues = malloc(sizeof(TStackElement)); // Alokujeme první prvek 
		assert(stack_invariant(aStack));
		if (!aStack->iValues)
			return false;
		assert(stack_invariant(aStack));
		aStack->iCapacity = 1;
		assert(stack_invariant(aStack));
	}
	if (aStack->iCount == aStack->iCapacity) { // Pokud je zásobník plný X/X
		TStackElement* newArray = calloc(2 * aStack->iCapacity, sizeof(TStackElement)); // Alokujeme dvonásobek kapacity
		if (!newArray)
			return false;
		for (size_t i = 0; i < aStack->iCount; i++) { // Zkopírujeme stáré prvky do nového zásobníku 
			newArray[i] = aStack->iValues[i];
		}
		free(aStack->iValues); // Smažeme starý zásobník 
		assert(stack_invariant(aStack));
		aStack->iValues = newArray; // Aktualizujeme ukazatel iValues
		assert(stack_invariant(aStack));
		aStack->iCapacity *= 2;
		assert(stack_invariant(aStack));
	}
	assert(stack_invariant(aStack));
	aStack->iValues[aStack->iCount++] = aValue;
	assert(stack_invariant(aStack));
	return true;
	}

bool stack_pop(struct TStack *aStack)
	{
	if (stack_is_empty(aStack))
		return false;
	assert(stack_invariant(aStack));
	aStack->iCount--;
	if (aStack->iCount < aStack->iCapacity / 4) { // Pokud je v zásobníku méně jak 25% prvků
		TStackElement* newArray = calloc(aStack->iCapacity / 2, sizeof(TStackElement));  // Alokujeme polovinu kapacity
		if (!newArray)
			return false;
		for (size_t i = 0; i < aStack->iCount; i++) { 
			newArray[i] = aStack->iValues[i];
		}
		free(aStack->iValues); 
		assert(stack_invariant(aStack));
		aStack->iValues = newArray; 
		assert(stack_invariant(aStack));
		aStack->iCapacity /= 2;
		assert(stack_invariant(aStack));
	}
	return true;
	}

void stack_destroy(struct TStack *aStack)
	{
	if (!aStack)
		return;
	free(aStack->iValues);
	aStack->iCount = 0;
	aStack->iCapacity = 0;
	aStack->iValues = NULL;
	assert(stack_invariant(aStack));
	}

struct TStackIterator stack_iterator_begin(const struct TStack *aStack)
	{
	if(!stack_is_empty(aStack))
		return (struct TStackIterator) { .iStack = aStack, .iPos = aStack->iCount };
	return (struct TStackIterator) { .iStack = NULL, .iPos = 0 };
	}

bool stack_iterator_is_valid(const struct TStackIterator *aIter)
	{
	if (aIter)
		if (aIter->iStack)
			if (aIter->iPos > 0 && aIter->iPos <= aIter->iStack->iCount)
				return true;
	return false;
	}

bool stack_iterator_to_next(struct TStackIterator *aIter)
	{
	if (stack_iterator_is_valid(aIter))
		if (aIter->iStack) {
			--aIter->iPos;
			if (aIter->iPos > 0)  // Stále platná pozice?
				return true;
			else {
				aIter->iStack = NULL;  // Zruš propojení
				return false;
			}
		}
	return false;
	}

TStackElement stack_iterator_value(const struct TStackIterator *aIter)
	{
	if (stack_iterator_is_valid(aIter))
		return aIter->iStack->iValues[aIter->iPos - 1];
	return (TStackElement) { 0 };
	}
