/** \file TSet.c
 *  \brief Implementace API pro typ Set (realizace unordered množiny pomocí hašovací tabulky)
 *  \author Petyovský
 *  \version 2026
 *  $Id: TSet.c 3361 2026-04-20 14:36:03Z petyovsky $
 */

#include "TSet.h"
#include "check.h"

void set_init(struct TSet *aSet)
	{
	if(aSet)
		*aSet = (struct TSet) { .iSize = 0, .iHashTable = NULL };
	}

size_t set_size(const struct TSet *aSet)
	{
	if(aSet)
		return aSet->iSize;
	return 0;
	}

bool set_insert(struct TSet *aSet, TSetElement aValue)
	{
	if(!aSet)
		return false;

	assert((aSet->iHashTable && aSet->iSize) || (!aSet->iHashTable && !aSet->iSize));

	if(!aSet->iHashTable)
		if(!set_hash_table_init(&aSet->iHashTable, HASH_TABLE_INIT_SIZE))
			return false;

	if(!set_hash_table_insert(aSet->iHashTable, aValue))
		return false;

	++aSet->iSize;
	return true;
	}

bool set_erase(struct TSet *aSet, TSetElement aValue)
	{
	if(set_is_empty(aSet))
		return false;

	if(!set_hash_table_erase(aSet->iHashTable, aValue))
		return false;

	--aSet->iSize;
	if(!aSet->iSize)	// Set is now empty, so mark all hash table buckets as never used again
		{
		set_hash_table_destroy(aSet->iHashTable);
		aSet->iHashTable = NULL;
		}

	return true;
	}

bool set_is_element_of(const struct TSet *aSet, TSetElement aValue)
	{
	if(!aSet)
		return false;

	size_t pos = 0;
	return set_hash_table_search(aSet->iHashTable, aValue, &pos) == 0;
	}

struct TSetIterator set_find(const struct TSet *aSet, TSetElement aValue)
	{
	if(aSet)
		{
		size_t pos = 0;
		if(set_hash_table_search(aSet->iHashTable, aValue, &pos) == 0)
			return (struct TSetIterator) { .iSet = aSet, .iPos = pos };	// success
		}

	return (struct TSetIterator) { .iSet = NULL, .iPos = 0 };	// fail
	}

void set_destroy(struct TSet *aSet)
	{
	if(!aSet)
		return;

	set_hash_table_destroy(aSet->iHashTable);
	*aSet = (struct TSet) { .iSize = 0, .iHashTable = NULL };
	}

struct TSetIterator set_iterator_begin(const struct TSet *aSet)
	{
	if(aSet)
		if(aSet->iSize)
			return (struct TSetIterator) { .iSet = aSet, .iPos = set_hash_table_begin_pos(aSet->iHashTable) };

	return (struct TSetIterator) { .iSet = NULL, .iPos = 0 };
	}

bool set_iterator_is_valid(const struct TSetIterator *aIter)
	{
	if(aIter)
		if(aIter->iSet)
			return set_hash_table_is_valid_pos(aIter->iSet->iHashTable, aIter->iPos);

	return false;
	}

bool set_iterator_to_next(struct TSetIterator *aIter)
	{
	bool valid = set_iterator_is_valid(aIter);
	if(valid)
		{
		aIter->iPos = set_hash_table_next_pos(aIter->iSet->iHashTable, aIter->iPos);
		valid = set_hash_table_is_valid_pos(aIter->iSet->iHashTable, aIter->iPos);
		}

	if(!valid)
		*aIter = (struct TSetIterator) { .iSet = NULL, .iPos = 0 };

	return valid;
	}

TSetElement set_iterator_value(const struct TSetIterator *aIter)
	{
	if(set_iterator_is_valid(aIter))
		return set_hash_table_value_at_pos(aIter->iSet->iHashTable, aIter->iPos);

	return (TSetElement) { 0 };
	}

bool set_iterator_is_equal(const struct TSetIterator *aLeftIter, const struct TSetIterator *aRightIter)
	{
	const bool left_iter_is_valid = set_iterator_is_valid(aLeftIter);
	const bool right_iter_is_valid = set_iterator_is_valid(aRightIter);

	if(left_iter_is_valid != right_iter_is_valid)
		return false;

	if(!left_iter_is_valid && !right_iter_is_valid)
		return true;

	return (aLeftIter->iSet == aRightIter->iSet) && (aLeftIter->iPos == aRightIter->iPos);
	}
