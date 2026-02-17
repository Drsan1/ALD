/** \file TVector.c
 *  \brief Implementace Basic Vector API
 *  \author Petyovský
 *  \version 2026
 *  $Id: TVector.c 3192 2026-02-08 18:45:10Z petyovsky $
 */

#include <stdlib.h>
#include <assert.h>
#include "TVector.h"
#include "check.h"

[[nodiscard]] static inline bool vector_invariant(const struct TVector *aVector)
	{
	return (aVector->iValues && aVector->iSize) || (!aVector->iValues && !aVector->iSize);
	}

bool vector_init(struct TVector *aVector, size_t aSize)
	{
	if (!aVector) 
		return false;
	if (!aSize) {
		//aVector->iSize = aSize; // = 0
		//aVector->iValues = NULL;
		*aVector = (struct TVector){.iSize = aSize, .iValues = NULL};
		assert(vector_invariant(aVector));
		return true;
	}
	TVectorElement *ptr = calloc(aSize, sizeof(TVectorElement));
	if (!ptr)
		return false;
	*aVector = (struct TVector){ .iSize = aSize, .iValues = ptr };
	assert(vector_invariant(aVector));
	return true;
	}

bool vector_init_random(struct TVector *aVector, size_t aSize)
	{
	return false;
	}

bool vector_init_file(struct TVector *aVector, FILE *aInputFile)
	{
	return false;
	}

bool vector_clone(const struct TVector *aVector, struct TVector *aVectorClone)
	{
	if (!aVector || !aVectorClone)
		return false;

	assert(vector_invariant(aVector));

	if (aVector == aVectorClone)
		return true;
	assert(vector_invariant(aVectorClone));
	
	if (aVector->iSize == 0) {
		// dealokuju klon
		free(aVector->iValues);
		*aVectorClone = (struct TVector){ .iSize = 0, .iValues = NULL };
		assert(vector_invariant(aVectorClone));
		return true;
	}
	if (aVector->iSize != aVectorClone->iSize) {
		free(aVectorClone->iValues);
		TVectorElement *ptr = malloc(aVector->iSize * sizeof(TVectorElement));
		aVectorClone->iValues = ptr;
		aVectorClone->iSize = aVector->iSize;
	}
	for (size_t i = 0; i < aVector->iSize; i++)
	{
		aVectorClone->iValues[i] = aVector->iValues[i];
	}
	assert(vector_invariant(aVectorClone));
	return true;
	}

bool vector_store_file(const struct TVector *aVector, FILE *aOutputFile)
	{
	return false;
	}

TVectorElement vector_value(const struct TVector *aVector, size_t aPos)
	{
	assert(aVector);
	assert(vector_invariant(aVector));
	assert(aPos < aVector->iSize);
	return aVector->iValues[aPos];
	}

void vector_set_value(struct TVector *aVector, size_t aPos, TVectorElement aValue)
	{
	assert(aVector);
	assert(vector_invariant(aVector));
	assert(aPos < aVector->iSize);
	aVector->iValues[aPos] = aValue;
	}

size_t vector_size(const struct TVector *aVector)
	{
	if (!aVector)
		return false;
	assert(vector_invariant(aVector));
	return aVector->iSize;
	}

bool vector_resize(struct TVector *aVector, size_t aNewSize)
	{
	if (!aVector)
		return false;
	assert(vector_invariant(aVector));
	if (aVector->iSize == aNewSize)
		return true;
	if (aNewSize == 0) {
		free(aVector->iValues);
		*aVector = (struct TVector){ .iSize = 0, .iValues = NULL };
		assert(vector_invariant(aVector));
		return true;
	}
	TVectorElement* ptr = calloc(aNewSize, sizeof(TVectorElement));
	if (!ptr)
		return false;
	size_t iter = (aNewSize > aVector->iSize) ? (aVector->iSize) : (aNewSize);
	for (size_t i = 0; i < iter; i++)
	{
		ptr[i] = aVector->iValues[i];
	}
	free(aVector->iValues);
	*aVector = (struct TVector){ .iSize = aNewSize, .iValues = ptr };
	assert(vector_invariant(aVector));
	return true;
	}

void vector_destroy(struct TVector *aVector)
	{
	if (!aVector)
		return;
	free(aVector->iValues);
	*aVector = (struct TVector){ .iSize = 0, .iValues = NULL };
	assert(vector_invariant(aVector));
	}

struct TVectorIterator vector_iterator_begin(const struct TVector *aVector)
	{
	return (struct TVectorIterator) { .iVector = NULL, .iPos = 0 };
	}

struct TVectorIterator vector_iterator_pos(const struct TVector *aVector, size_t aPos)
	{
	return (struct TVectorIterator) { .iVector = NULL, .iPos = 0 };
	}

struct TVectorIterator vector_iterator_last(const struct TVector *aVector)
	{
	return (struct TVectorIterator) { .iVector = NULL, .iPos = 0 };
	}

bool vector_iterator_is_valid(const struct TVectorIterator *aIter)
	{
	return false;
	}

bool vector_iterator_to_next(struct TVectorIterator *aIter)
	{
	return false;
	}

bool vector_iterator_to_prev(struct TVectorIterator *aIter)
	{
	return false;
	}

TVectorElement vector_iterator_value(const struct TVectorIterator *aIter)
	{
	return (TVectorElement) { 0 };
	}

bool vector_iterator_set_value(const struct TVectorIterator *aIter, TVectorElement aValue)
	{
	return false;
	}
