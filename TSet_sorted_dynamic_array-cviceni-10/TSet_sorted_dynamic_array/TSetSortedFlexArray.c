/** \file TSetSortedFlexArray.c
 *  \brief Implementace API pro typ SetSortedFlexArray (Flexibilní, tzv. otevřené, pole pro realizaci množiny pomocí setříděného dynamicky alokovaného pole)
 *  \author Petyovský
 *  \version 2026
 *  $Id: TSetSortedFlexArray.c 3361 2026-04-20 14:36:03Z petyovsky $
 */

#define LINEAR_SEARCH_OPTIMIZED	1	///< Volba pro vyhledávání s optimalizací dřívějšího ukončení
#define BINARY_SEARCH			2	///< Volba pro binární vyhledávání

#define FLEX_ARRAY_SEARCH_VARIANT	LINEAR_SEARCH_OPTIMIZED	///< Varianta využívající lineární vyhledávání s optimalizací ve funkci: `set_flex_array_search()`
//#define FLEX_ARRAY_SEARCH_VARIANT	BINARY_SEARCH			///< Varianta využívající binární vyhledávání ve funkci: `set_flex_array_search()`

#include <stdlib.h>
#include <assert.h>
#include "TSetSortedFlexArray.h"
#include "check.h"

/** \brief Úplná definice privátního typu SetSortedFlexArray
 *  \details Privátní typ SetSortedFlexArray (nedostupný mimo funkce z tohoto zdrojového souboru) reprezentuje typ pro tzv. flexibilní (otevřené) pole hodnot typu SetElement.
 */
struct TSetSortedFlexArray
	{
	size_t iCapacity;			///< Skutečně naalokovaná kapacita prvků vnitřního dynamického pole
	TSetElement iElement[];		///< Flexibilní (otevřené) pole hodnot typu SetElement (nedostupné mimo funkce ze souboru TSetSortedFlexArray.c)
	};

int set_flex_array_search(const struct TSetSortedFlexArray *aFlexArray, TSetElement aValue, size_t *aPosPtr, size_t aSize)
	{
	assert(aPosPtr);

	if(!aSize)
		{
		*aPosPtr = 0;
		return -2;
		}

	assert(aFlexArray);
	assert(aFlexArray->iCapacity >= aSize);

#if FLEX_ARRAY_SEARCH_VARIANT == LINEAR_SEARCH_OPTIMIZED
	/* Zde implementujte lineární vyhledávání s optimalizací dřívějšího ukončení */
	int cmp = -2;
	for (size_t i = 0; i < aSize; ++i)
	{ 
		cmp = set_element_comparator(&aValue, &aFlexArray->iElement[i]);
		if (cmp <= 0)
		{
			*aPosPtr = i;
			return cmp; 
		}
	}
	*aPosPtr = aSize - 1;
	return cmp;

#elif FLEX_ARRAY_SEARCH_VARIANT == BINARY_SEARCH
	/* Zde implementujte binární vyhledávání */
	size_t l = 0;
	size_t r = aSize - 1;
	size_t m = 0;
	int cmp = 0;

	while (l <= r)
		{
		m = l + (r - l) / 2;
		cmp = set_element_comparator(&aValue, &aFlexArray->iElement[m]);

		if (cmp == 0)
			{
			*aPosPtr = m;
			return cmp;
			}
		else if (cmp < 0)
			{
			if (m == 0)
				break;
			r = m - 1;
			}
		else
			{
			l = m + 1;
			}
		}

	*aPosPtr = m;
	return cmp;

#else
	#error Missing correct FLEX_ARRAY_SEARCH_VARIANT symbol definition
#endif
	}

bool set_flex_array_insert(struct TSetSortedFlexArray **aFlexArrayPtr, TSetElement aValue, size_t aSize)
	{
	assert(aFlexArrayPtr);
	struct TSetSortedFlexArray *act_array = *aFlexArrayPtr;

	/* Zde implementujte amortizované vložení prvku do flexibilního dynamicky alokovaného pole na správnou pozici. */
	/* Využijte servisní funkci: set_flex_array_search() */
	size_t pos = 0;
	int cmp = set_flex_array_search(act_array, aValue, &pos, aSize);
	if (cmp == 0)
		return false;
	if (cmp == 1)
		pos++;
	if ((!aSize) || (aSize >= act_array->iCapacity))
	{
		const size_t new_capacity = (aSize) ? (2 * act_array->iCapacity) : (2);
		struct TSetSortedFlexArray* new_array =
			malloc(offsetof(struct TSetSortedFlexArray, iElement) + new_capacity * sizeof(TSetElement));
		if (!new_array)
			return false;
		new_array->iCapacity = new_capacity;
		for (size_t i = 0; i < pos; ++i)
			new_array->iElement[i] = act_array->iElement[i];
		new_array->iElement[pos] = aValue;
		for (size_t i = pos; i < aSize; ++i)
			new_array->iElement[i + 1] = act_array->iElement[i];
		*aFlexArrayPtr = new_array;
		free(act_array);
		return true;
	}
	for (size_t i = aSize; i > pos; --i)
		act_array->iElement[i] = act_array->iElement[i - 1];
	act_array->iElement[pos] = aValue;
	return true;
	}

bool set_flex_array_erase(struct TSetSortedFlexArray **aFlexArrayPtr, TSetElement aValue, size_t aSize)
	{
	assert(aFlexArrayPtr);
	assert(aSize);

	struct TSetSortedFlexArray *act_array = *aFlexArrayPtr;

	/* Zde implementujte amortizované odebrání prvku ze správného místa ve flexibilním dynamicky alokovaném poli. */
	/* Využijte servisní funkci: set_flex_array_search() */
	size_t pos = 0;
	if (set_flex_array_search(act_array, aValue, &pos, aSize) != 0)
		return false;

	size_t new_size = aSize - 1;
	if (!new_size)
	{
		*aFlexArrayPtr = NULL;
		free(act_array);
		return true;
	}

	if (act_array->iCapacity >= 4 * new_size)
	{
		size_t new_capacity = act_array->iCapacity / 2;
		struct TSetSortedFlexArray* new_array =
			malloc(offsetof(struct TSetSortedFlexArray, iElement) + new_capacity * sizeof(TSetElement));

		if (!new_array)
			return false;

		new_array->iCapacity = new_capacity;
		for (size_t i = 0; i < pos; ++i)
			new_array->iElement[i] = act_array->iElement[i];

		for (; pos < new_size; ++pos)
			new_array->iElement[pos] = act_array->iElement[pos + 1];

		*aFlexArrayPtr = new_array;
		free(act_array);

		return true;
	}

	for (; pos < new_size; ++pos)
		act_array->iElement[pos] = act_array->iElement[pos + 1];

	return true;
	}

void set_flex_array_destroy(struct TSetSortedFlexArray *aFlexArray)
	{
	free(aFlexArray);
	}

size_t set_flex_array_begin_pos(const struct TSetSortedFlexArray *aFlexArray)
	{
	return 0;
	}

bool set_flex_array_is_valid_pos(const struct TSetSortedFlexArray *aFlexArray, size_t aPos, size_t aSize)
	{
	if(aFlexArray)
		return (aSize <= aFlexArray->iCapacity) && (aPos < aSize);

	return false;
	}

size_t set_flex_array_next_pos(const struct TSetSortedFlexArray *aFlexArray, size_t aPos)
	{
	return ++aPos;
	}

TSetElement set_flex_array_value_at_pos(const struct TSetSortedFlexArray *aFlexArray, size_t aPos)
	{
	assert(aFlexArray);
	assert(aPos < aFlexArray->iCapacity);

	return aFlexArray->iElement[aPos];
	}
