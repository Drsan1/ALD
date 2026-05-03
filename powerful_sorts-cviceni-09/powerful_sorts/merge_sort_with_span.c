/** \file merge_sort_with_span.c
 *  \brief Implementace API výkonnější metody třídění slučováním (Merge sort), využívající rekurzi a datový typ Span
 *  \author Petyovský
 *  \version 2026
 *  $Id: merge_sort_with_span.c 3339 2026-04-10 16:15:30Z petyovsky $
 */

#include "merge_sort_with_span.h"
#include "TSpan.h"

/** \brief Vytisknutí elementu Spanu pomocí Span iterátoru
 *  \details Tisk hodnoty elementu Spanu, na který ukazuje Span iterátor.
 *  \param[in] aIter Ukazatel na existující Span iterátor
 */
[[maybe_unused]] static void span_element_print(const struct TSpanIterator *aIter)
	{
	if(!vector_element_store_file(span_iterator_value(aIter), stdout))
		fputs("Function 'vector_element_store_file()' failed!\n", stderr);
	}

/** \brief Vnitřní a lokální funkce realizující algoritmus sloučení dvou již setříděných rozsahů (Spanů)
 *  \details Realizuje algoritmus sloučení dvou již setříděných rozsahů \p aFrom1 \p aFrom2 do výsledného rozsahu \p aTo.
 *  \param[in] aFrom1 První rozsah obsahující již setříděné vstupních elementy
 *  \param[in] aFrom2 Druhý rozsah obsahující již setříděné vstupních elementy
 *  \param[out] aTo Výstupní rozsah pro elementy z obou sloučených / zatříděných rozsahů \p aFrom1 a \p aFrom2
 */
static void mergify(const struct TSpan aFrom1, const struct TSpan aFrom2, struct TSpan aTo)
	{
//	fputs("Mergify:\n", stdout);
//	span_for_each(span_iterator_begin(&aFrom1), span_element_print);
//	fputs("| ", stdout);
//	span_for_each(span_iterator_begin(&aFrom2), span_element_print);
//	fputc('\n', stdout);

	const size_t from1_size = span_size(&aFrom1);
	const size_t from2_size = span_size(&aFrom2);
	size_t f1 = 0, f2 = 0, t = 0;

	for(; f1 < from1_size && f2 < from2_size; ++t)
		{
		if(span_compare_positions(&aFrom1, f1, &aFrom2, f2) <= 0)
			span_move_positions(&aTo, t, &aFrom1, f1++);
		else
			span_move_positions(&aTo, t, &aFrom2, f2++);
		}

	for(; f1 < from1_size; ++t)
		span_move_positions(&aTo, t, &aFrom1, f1++);

	for(; f2 < from2_size; ++t)
		span_move_positions(&aTo, t, &aFrom2, f2++);

//	span_for_each(span_iterator_begin(&aTo), span_element_print);
//	fputc('\n', stdout);
	}

/** \brief Vnitřní a lokální funkce realizující rekurzivní funkci pro metodu (Merge Sort)
 *  \details Realizuje rekurzivní funkci pro třídicí metodu (Merge Sort), setřídí elementy rozsahu \p aFrom a ukládá je do rozsahu \p aTo.
 *  \param[in] aFrom Vstupní rozsah říděných vstupních elementů
 *  \param[out] aTo Výstupní rozsah elementů pro setříděné elementy
 */
static void merge_sort_worker(struct TSpan aFrom, struct TSpan aTo)
	{
	if(span_size(&aFrom) < 2)
		return;

//	fputs("aFrom:\n", stdout);
//	span_for_each(span_iterator_begin(&aFrom), span_element_print);
//	fputc('\n', stdout);

	const size_t middle = span_size(&aTo) / 2;

//	{
//	fputs("Split:\n", stdout);
//	struct TSpan left = span_create_left_subspan(aTo, middle);
//	struct TSpan right = span_create_right_subspan(aTo, middle);
//
//	span_for_each(span_iterator_begin(&left), span_element_print);
//	fputs("| ", stdout);
//	span_for_each(span_iterator_begin(&right), span_element_print);
//	fputc('\n', stdout);
//	}

	struct TSpan from1 = span_create_left_subspan(aFrom, middle);
	merge_sort_worker(span_create_left_subspan(aTo, middle), from1);

	struct TSpan from2 = span_create_right_subspan(aFrom, middle);
	merge_sort_worker(span_create_right_subspan(aTo, middle), from2);

	mergify(from1, from2, aTo);
	}

void merge_sort_with_span(struct TVector *aVector)
	{
	if(vector_size(aVector) < 2)
		return;

	struct TVector work_vector = { 0 };
	if(!vector_attach_sortstats(aVector, &work_vector))	// Create sortstats relationship between `aVector` and `work_vector`
		return;

	if(!vector_clone(aVector, &work_vector))			// Copy all element values from `aVector` to the `work_vector`
		return;

	merge_sort_worker(span_create(work_vector), span_create(*aVector));

	vector_destroy(&work_vector);
	}
