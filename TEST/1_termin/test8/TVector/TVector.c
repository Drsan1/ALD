/** \file TVector.c
 *  \brief Implementace Basic Vector API
 *  \author Petyovský
 *  \version 2026
 *  $Id: TVector.c 3192 2026-02-08 18:45:10Z petyovsky $
 */

#include <stdlib.h>
#include <assert.h>
#include <math.h>
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
	if (!aVector)
		return false;
	if (!aSize) {
				*aVector = (struct TVector){ .iSize = aSize, .iValues = NULL };
		assert(vector_invariant(aVector));
		return true;
	}
	TVectorElement* ptr = calloc(aSize, sizeof(TVectorElement));
	if (!ptr)
		return false;
	for (size_t i = 0; i < aSize; i++)
		{
		ptr[i] = vector_element_random_value();
		}
	*aVector = (struct TVector){ .iSize = aSize, .iValues = ptr };
	assert(vector_invariant(aVector));
	return true;
	}

bool vector_init_file(struct TVector *aVector, FILE *aInputFile)
	{
	if (!aVector || !aInputFile)
		return false;
	size_t size = 0;
	if (fscanf(aInputFile, "%zu", &size) != 1)
		return false;
	if (size == 0) {
		*aVector = (struct TVector){ .iSize = 0, .iValues = NULL };
		return true;
	}
	TVectorElement* ptr = calloc(size, sizeof(TVectorElement));
	if (!ptr)
		return false;
	for (size_t i = 0; i < size; i++) {
		if (fscanf(aInputFile, TVECTOR_ELEMENT_FRMSTR, &ptr[i]) != 1) {
			free(ptr);
			return false;
		}
	}
	*aVector = (struct TVector){ .iSize = size, .iValues = ptr };
	assert(vector_invariant(aVector));
	return true;
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
		free(aVectorClone->iValues);
		*aVectorClone = (struct TVector){ .iSize = 0, .iValues = NULL };
		assert(vector_invariant(aVectorClone));
		return true;
	}
	if (aVector->iSize != aVectorClone->iSize) {
		free(aVectorClone->iValues);
		TVectorElement *ptr = malloc(aVector->iSize * sizeof(TVectorElement));
		if (!ptr)
			return false;
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
	if (!aVector || !aOutputFile)
		return false;
	assert(vector_invariant(aVector));
	if (fprintf(aOutputFile, "%zu\n", aVector->iSize) < 1)
		return false;
	for (size_t i = 0; i < aVector->iSize; i++) {
		if (fprintf(aOutputFile, TVECTOR_ELEMENT_FRMSTR "\n", aVector->iValues[i]) < 1) {
			return false;
		}
	}
	return true;
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
		return 0;
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

int vector_threeway_comparator(const struct TVector* aVector1, const struct TVector* aVector2) {
	if (!aVector1 || !aVector2)
		return 0;
	double size1 = 0;
	for (size_t i = 0; i < aVector1->iSize; i++) {
		size1 += aVector1->iValues[i] * aVector1->iValues[i];
	}
	size1 = sqrt(size1);

	double size2 = 0;
	for (size_t i = 0; i < aVector1->iSize; i++) {
		size2 += aVector1->iValues[i] * aVector1->iValues[i];
	}
	size2 = sqrt(size1);

	if (size1 < size2)
		return -1;
	if (size1 > size2)
		return 1;
	else 
		return 0;
}

struct TVectorIterator vector_iterator_begin(const struct TVector *aVector)
	{
	if (!aVector)
		return (struct TVectorIterator) { .iVector = NULL, .iPos = 0 };
	return (struct TVectorIterator) { .iVector = (struct TVector*)aVector, .iPos = 0 };
	}

struct TVectorIterator vector_iterator_pos(const struct TVector *aVector, size_t aPos)
	{
	if (!aVector || aPos >= aVector->iSize)
		return (struct TVectorIterator) { .iVector = NULL, .iPos = 0 };
	return (struct TVectorIterator) { .iVector = (struct TVector*)aVector, .iPos = aPos };
	}

struct TVectorIterator vector_iterator_last(const struct TVector *aVector)
	{
	if (!aVector || aVector->iSize == 0)
		return (struct TVectorIterator) { .iVector = NULL, .iPos = 0 };
	return (struct TVectorIterator) { .iVector = (struct TVector*)aVector, .iPos = aVector->iSize - 1 };
	}

bool vector_iterator_is_valid(const struct TVectorIterator *aIter)
	{
	if (!aIter || !aIter->iVector)
		return false;
	return aIter->iPos < aIter->iVector->iSize;
	}

bool vector_iterator_to_next(struct TVectorIterator *aIter)
	{
	if (!vector_iterator_is_valid(aIter))
		return false;
	++aIter->iPos;
	return vector_iterator_is_valid(aIter);
	}

bool vector_iterator_to_prev(struct TVectorIterator *aIter)
	{
	if (!vector_iterator_is_valid(aIter))
		return false;
	if (aIter->iPos == 0)
		return false;
	--aIter->iPos;
	return true;
	}

TVectorElement vector_iterator_value(const struct TVectorIterator *aIter)
	{
	if (!vector_iterator_is_valid(aIter))
		return (TVectorElement) { 0 };
	return aIter->iVector->iValues[aIter->iPos];
	}

bool vector_iterator_set_value(const struct TVectorIterator *aIter, TVectorElement aValue)
	{
	if (!vector_iterator_is_valid(aIter))
		return false;
	aIter->iVector->iValues[aIter->iPos] = aValue;
	return true;
	}
