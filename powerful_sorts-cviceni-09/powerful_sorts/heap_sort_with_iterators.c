/** \file heap_sort_with_iterators.c
 *  \brief Implementace API výkonnější metody třídění (Heap sort), využívající iterátorové API pro přístup k uzlům haldy
 *  \author Petyovský
 *  \version 2026
 *  $Id: heap_sort_with_iterators.c 3342 2026-04-13 14:19:07Z petyovsky $
 */

#include "heap_sort_with_iterators.h"
#include "THeap.h"

/** \brief Vnitřní a lokální funkce realizující algoritmus vložení hodnoty do zvoleného uzlu haldy, při zachování podmínek haldy
 *  \details Lokální funkce implementuje algoritmus vložení hodnoty do zvoleného uzlu haldy, při zachování podmínek haldy (prosívání hodnoty směrem k listům).
 *  \param[in] aParent Iterátor na uzel haldy určený pro vložení hodnoty \p aValue (hodnota v tomto uzlu bude přepsána)
 *  \param[in] aValue Hodnota vkládáná do haldy
 */
static inline void heap_iterator_alg_sift_down(struct THeapIterator aParent, THeapElement aValue)
	{
	for (struct THeapIterator it = aParent; heap_iterator_to_left(&it); aParent = it){
		struct THeapIterator itr = it;
		if (heap_iterator_to_next(&itr) && heap_compare_with_iterators(&it, &itr) < 0)
			it = itr;
		if (heap_compare_with_iterator_value(&it, aValue) <= 0)
			break;
		heap_move_with_iterators(&aParent, &it);
	}
	heap_iterator_set_value(&aParent, aValue);
	}

/** \brief Vnitřní a lokální funkce realizující algoritmus uspořádání elementů vektoru do haldy
 *  \details Lokální funkce implementuje algoritmus uspořádání elementů vektoru do haldy, pomocí volání funkce `sift_down()`.
 *  \param[in,out] aHeap Ukazatel na vytvářenou haldu
 */
static inline void heap_iterator_alg_heapify(struct THeap *aHeap)
	{
	struct THeapIterator parent = heap_iterator_last(aHeap);
	if (!heap_iterator_to_parent(&parent))
		return;
	do {
		heap_iterator_alg_sift_down(parent, heap_iterator_value(&parent));
//		fputs("Heapify:", stdout);
//		heap_dump(aHeap, stdout);
	} while (heap_iterator_to_prev(&parent));

	}

void heap_sort_with_iterators(struct TVector *aVector)
	{
//	for(size_t i = 0; i < vector_size(aVector); ++i)
//		if(!vector_element_store_file(aVector->iValues[i], stdout))
//			fputs("Function 'vector_element_store_file()' failed!\n", stderr);
//	putchar('\n');

	if(vector_size(aVector) < 2)
		return;

	//auto heap = heap_create(*aVector);		// `auto` since C23
	struct THeap heap = heap_create(*aVector);

//	heap_dump(&heap, stdout);

	heap_iterator_alg_heapify(&heap);

//	heap_dump(&heap, stdout);

//	fputs("Heapify stats:\n", stdout);
//	if(!vector_print_sortstats(aVector, stdout))
//		fputs("Function 'vector_print_sortstats()' failed!\n", stderr);

	const struct THeapIterator root = heap_iterator_begin(&heap);
	size_t heapsize = heap_size(&heap);

	while(heapsize > 1)
		{
		const struct THeapIterator last = heap_iterator_last(&heap);

//		heap_dump(&heap, stdout);

		const THeapElement value = heap_iterator_value(&last);
		heap_move_with_iterators(&last, &root);

		heapsize = heap_decrement_size(&heap);

//		fprintf(stdout, "Value: %" TVECTOR_ELEMENT_FRMSTR "\n", value);
//		heap_dump(&heap, stdout);

		heap_iterator_alg_sift_down(root, value);

//		heap_dump(&heap, stdout);
		}
	}
