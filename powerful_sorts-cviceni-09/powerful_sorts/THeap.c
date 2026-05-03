/** \file THeap.c
 *  \brief Implementace API typu Heap pro přístup k elementům vektoru jako k uzlům haldy (Sortable Vector with shareable sortstats)
 *  \author Petyovský
 *  \version 2026
 *  $Id: THeap.c 3295 2026-03-20 15:34:49Z petyovsky $
 */

#include "THeap.h"

bool heap_iterator_to_left(struct THeapIterator *aIter)
	{
	if(!heap_iterator_is_valid(aIter))
		return false;

	aIter->iVectorIter.iPos = heap_pos_to_left(aIter->iVectorIter.iPos);
	if(aIter->iVectorIter.iPos >= aIter->iVectorIter.iVector->iSize)
		{
		*aIter = (struct THeapIterator) { .iVectorIter = (struct TVectorIterator) { .iVector = NULL, .iPos = 0 } };
		return false;
		}

	return true;
	}

bool heap_iterator_to_right(struct THeapIterator *aIter)
	{
	if(!heap_iterator_is_valid(aIter))
		return false;

	aIter->iVectorIter.iPos = heap_pos_to_right(aIter->iVectorIter.iPos);
	if(aIter->iVectorIter.iPos >= aIter->iVectorIter.iVector->iSize)
		{
		*aIter = (struct THeapIterator) { .iVectorIter = (struct TVectorIterator) { .iVector = NULL, .iPos = 0 } };
		return false;
		}

	return true;
	}

bool heap_iterator_to_parent(struct THeapIterator *aIter)
	{
	if(!heap_iterator_is_valid(aIter))
		return false;

	if(!aIter->iVectorIter.iPos)
		{
		*aIter = (struct THeapIterator) { .iVectorIter = (struct TVectorIterator) { .iVector = NULL, .iPos = 0 } };
		return false;
		}

	aIter->iVectorIter.iPos = heap_pos_to_parent(aIter->iVectorIter.iPos);
	return true;
	}

void heap_dump(const struct THeap *aHeap, FILE *aOutputFile)
	{
	const size_t n = heap_size(aHeap);
	if(n > 99)		// max elements
		return;

	fprintf(aOutputFile, "\nHeap view (%zu elements):\n", n);
	size_t h = 0;
	for(size_t x = n; x; ++h, x >>= 1);
	//printf("%zu\n", h);

	for(size_t i = 0; h; --h, ++i)
		{
		const size_t k = ((size_t)1 << i) - 1;
		const size_t l = (k << 1);	//((size_t)2 << i) - 2;
		const unsigned sp = (1 << h) - 2;

		//printf("<%zu - %zu>\n", k, l);
		//printf("sp: %u\n", sp);
		for(size_t j = k; (j <= l) && (j < n); ++j)
			fprintf(aOutputFile, "%*s"   "%2" THEAP_ELEMENT_FRMSTR " "   "%*s", sp, "", aHeap->iVector.iValues[j], sp + 1, "");

		fputc('\n', aOutputFile);
		}

	fprintf(aOutputFile, "\nVector view (%zu elements):\n", n);
	for(size_t j = 0; j < n; ++j)
		fprintf(aOutputFile, "%2" THEAP_ELEMENT_FRMSTR " "   "%*s", aHeap->iVector.iValues[j], 1, "");

	fputs("\n\n----------\n", aOutputFile);
	}
