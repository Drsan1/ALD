/** \file advanced_sorts_with_span.c
 *  \brief Implementace API účinnějších metod třídění využívající typ Span
 *  \author Petyovský
 *  \version 2026
 *  $Id: advanced_sorts_with_span.c 3317 2026-04-02 17:27:02Z petyovsky $
 */

#include "advanced_sorts_with_span.h"
#include "TSpan.h"

static struct TVectorIterator Vector_begin_it = { 0 };		///< Bohužel z důvodů tisku hodnot elementů, je zde třeba statická proměnná :-(

/** \brief Formátovaný tisk hodnoty elementu za předpokladu platné hodnoty predikátu
 *  \details Vytiskne formátovaně hodnotu elementu pokud je parametr \p aCondition platný, jinak se vytisknou zástupné znaky.
 *  \param[in] aCondition pokud je \c true bude hodnota vytisknuta, jinak se vytisknou zástupné znaky
 *  \param[in] aIter Ukazatel na iterátor určující element k vytisknutí
 */
[[maybe_unused]] static void vector_element_print_if(bool aCondition, const struct TVectorIterator *aIter)
	{
	if(!aCondition)
		fputs(". ", stdout);
	else
		if(!vector_element_store_file(vector_iterator_value(aIter), stdout))
			fputs("Function 'vector_element_store_file()' failed!\n", stderr);
	}

/** \brief Vnitřní a lokální funkce realizující rekurzivní funkci pro metodu (Quick Sort)
 *  \details Realizuje rekurzivní funkci pro třídicí metodu (Quick Sort) pro omezený rozsah elementů vektoru.
 *  \param[in,out] aSpan Omezený rozsah elementů vektoru
 */
static void quick_sort_with_span_worker(struct TSpan aSpan)
	{
	size_t size = span_size(&aSpan);
	if (size < 2)
		return;

	size_t left = 0;
	size_t right = size - 1;
	TVectorElement pivotValue = span_value(&aSpan, right / 2);

	while (left <= right)
		{
		while (span_compare_position_value(&aSpan, left, pivotValue) < 0)
			left++;
		while (span_compare_position_value(&aSpan, right, pivotValue) > 0)
			right--;

		if (left <= right)
			{
			span_swap_positions(&aSpan, left, &aSpan, right);
			left++;
			if (right > 0)
				right--;
			else
				break;
			}
		}

	if (right > 0)
		quick_sort_with_span_worker(span_create_left_subspan(aSpan, right + 1));
	if (left < size - 1)
		quick_sort_with_span_worker(span_create_right_subspan(aSpan, left));
	}

void quick_sort_with_span(struct TVector *aVector)
	{
	if(vector_size(aVector) < 2) // Výraz v podmínce vhodně upravte :-)
		return;

	Vector_begin_it = vector_iterator_begin(aVector);

	quick_sort_with_span_worker(span_create(*aVector));
	}
