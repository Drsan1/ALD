/** \file heap_sort.c
 *  \brief Implementace API výkonnější metody třídění (Heap sort), využívající poziční API pro přístup k uzlům haldy
 *  \author Petyovský
 *  \version 2026
 *  $Id: heap_sort.c 3342 2026-04-13 14:19:07Z petyovsky $
 */

#include "heap_sort.h"
#include "THeap.h"

/** \brief Vnitřní a lokální funkce zajišťující algoritmus vložení hodnoty do zvoleného uzlu haldy, při zachování podmínek haldy
 *  \details Lokální funkce implementuje algoritmus vložení hodnoty do zvoleného uzlu haldy, při zachování podmínek haldy (prosívání hodnoty směrem k listům).
 *  \param[in,out] aHeap Ukazatel na vytvářenou haldu
 *  \param[in] aParentPos Index uzlu určeného pro vložení hodnoty \p aValue (hodnota v tomto uzlu bude přepsána).
 *  \param[in] aValue Hodnota vkládáná do haldy
 */
static inline void heap_alg_sift_down(struct THeap *aHeap, size_t aParentPos, THeapElement aValue)
	{
	size_t heap_sz = heap_size(aHeap);
	size_t current = aParentPos;
	size_t child = 2 * current + 1;

	while(child < heap_sz)
		{
		if((child + 1 < heap_sz) && (heap_compare_positions(aHeap, child, aHeap, child + 1) < 0))
			++child;

		if(heap_compare_position_value(aHeap, child, aValue) <= 0)
			break;

		heap_move_positions(aHeap, current, aHeap, child);
		current = child;
		child = 2 * current + 1;
		}
	heap_set_value(aHeap, current, aValue);
	}

/** \brief Vnitřní a lokální funkce realizující algoritmus uspořádání elementů vektoru do haldy
 *  \details Lokální funkce implementuje algoritmus uspořádání elementů vektoru do haldy, pomocí volání funkce `sift_down()`.
 *  \param[in,out] aHeap Ukazatel na vytvářenou haldu
 */
static inline void heap_alg_heapify(struct THeap *aHeap)
	{
	size_t heap_sz = heap_size(aHeap);
	if(heap_sz < 2)
		return;

	for(size_t i = heap_sz / 2; i > 0; --i)
		{
		size_t parent_pos = i - 1;
		THeapElement val = heap_value(aHeap, parent_pos);
		heap_alg_sift_down(aHeap, parent_pos, val);
		}
	}

void heap_sort(struct TVector *aVector)
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

	heap_alg_heapify(&heap);

//	heap_dump(&heap, stdout);
	
//	fputs("Heapify stats:\n", stdout);
//	if(!vector_print_sortstats(aVector, stdout))
//		fputs("Function 'vector_print_sortstats()' failed!\n", stderr);

	const size_t root_pos = 0;
	size_t heapsize = heap_size(&heap);

	while(heapsize > 1)
		{
		const size_t last_pos = heapsize - 1;

//		heap_dump(&heap, stdout);

		const THeapElement value = heap_value(&heap, last_pos);
		heap_move_positions(&heap, last_pos, &heap, root_pos);

		heapsize = heap_decrement_size(&heap);

//		fprintf(stdout, "Value: %" TVECTOR_ELEMENT_FRMSTR "\n", value);
//		heap_dump(&heap, stdout);

		heap_alg_sift_down(&heap, root_pos, value);

//		heap_dump(&heap, stdout);
		}
	}
