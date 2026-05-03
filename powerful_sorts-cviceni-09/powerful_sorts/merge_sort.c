/** \file merge_sort.c
 *  \brief Implementace API výkonnější metody třídění slučováním (Merge sort), využívající rekurzi
 *  \author Petyovský
 *  \version 2026
 *  $Id: merge_sort.c 3339 2026-04-10 16:15:30Z petyovsky $
 */

#include "merge_sort.h"

/** \brief Vytisknutí elementu vektoru pomocí iterátoru
 *  \details Tisk hodnoty elementu vektoru, na který ukazuje iterátor.
 *  \param[in] aIter Ukazatel na existující iterátor
 */
[[maybe_unused]] static void vector_element_print(const struct TVectorIterator *aIter)
	{
	if(!vector_element_store_file(vector_iterator_value(aIter), stdout))
		fputs("Function 'vector_element_store_file()' failed!\n", stderr);
	}

/** \brief Vnitřní a lokální funkce realizující algoritmus sloučení dvou již setříděných rozsahů z vektoru `aFromVector` do vektoru `aToVector`
 *  \details Realizuje algoritmus sloučení dvou již setříděných rozsahů `<aBegin,aMiddle)` a `<aMiddle,aEnd)` z vektoru `aFromVector` do vektoru `aToVector`.
 *  \param[in] aFromVector Ukazatel na vstupní vektor obsahující již dva setříděné rozsahy elementů
 *  \param[in] aBegin Index prvního rozsahu ve vektoru \p aFromVector
 *  \param[in] aMiddle Index prvního a druhého rozsahu ve vektoru \p aFromVector
 *  \param[in] aEnd Index druhého rozsahu ve vektoru \p aFromVector
 *  \param[out] aToVector Ukazatel na sloučený (zatříděný) vektor elementů
 */
static void mergify(const struct TVector *aFromVector, size_t aBegin, size_t aMiddle, size_t aEnd, struct TVector *aToVector)
	{
//	fputs("Mergify:\n", stdout);
//	vector_for_range(vector_iterator_pos(aFromVector, aBegin), vector_iterator_pos(aFromVector, aMiddle - 1), vector_element_print);
//	fputs("| ", stdout);
//	vector_for_range(vector_iterator_pos(aFromVector, aMiddle), vector_iterator_pos(aFromVector, aEnd - 1), vector_element_print);
//	fputc('\n', stdout);

	size_t f1 = aBegin;
	size_t f2 = aMiddle;
	size_t t = aBegin;

	for (; f1 < aMiddle && f2 < aEnd; ++t) {
		if (vector_compare_positions(aFromVector, f1, aFromVector, f2) <= 0)
			vector_move_positions(aToVector, t, aFromVector, f1++);
		else
			vector_move_positions(aToVector, t, aFromVector, f2++);
	}

	for (; f1 < aMiddle; ++t)
			vector_move_positions(aToVector, t, aFromVector, f1++);

	for (; f2 < aEnd; ++t)
		vector_move_positions(aToVector, t, aFromVector, f2++);

//	vector_for_range(vector_iterator_pos(aToVector, aBegin), vector_iterator_pos(aToVector, aEnd - 1), vector_element_print);
//	fputc('\n', stdout);
	}

/** \brief Vnitřní a lokální funkce realizující rekurzivní funkci pro metodu (Merge Sort)
 *  \details Realizuje rekurzivní funkci pro třídicí metodu (Merge Sort), setřídí elementy vektoru \p aFromVector v rozsahu pozic `<aBegin,aEnd)` a ukládá je do vektoru \p aToVector.
 *  \param[in] aFromVector Ukazatel na tříděný vektor hodnot
 *  \param[in] aBegin Index levého elementu v rozsahu vektoru \p aFromVector
 *  \param[in] aEnd Index pravého elementu v rozsahu vektoru \p aFromVector
 *  \param[out] aToVector Ukazatel na tříděný vektor hodnot
 */
static void merge_sort_worker(struct TVector *aFromVector, size_t aBegin, size_t aEnd, struct TVector *aToVector)
	{
	if((aBegin >= aEnd) || (aEnd - aBegin < 2))
		return;

//	fputs("aFrom:\n", stdout);
//	vector_for_range(vector_iterator_pos(aFromVector, aBegin), vector_iterator_pos(aFromVector, aEnd - 1), vector_element_print);
//	fputc('\n', stdout);

	const size_t middle = aBegin + (aEnd - aBegin) / 2;

//	{
//	fputs("Split:\n", stdout);
//	vector_for_range(vector_iterator_pos(aFromVector, aBegin), vector_iterator_pos(aFromVector, middle - 1), vector_element_print);
//	fputs("| ", stdout);
//	vector_for_range(vector_iterator_pos(aFromVector, middle), vector_iterator_pos(aFromVector, aEnd - 1), vector_element_print);
//	fputc('\n', stdout);
//	}

	merge_sort_worker(aToVector, aBegin, middle, aFromVector);
	merge_sort_worker(aToVector, middle, aEnd, aFromVector);

	mergify(aFromVector, aBegin, middle, aEnd, aToVector);

//	fputs("aToVector:\n", stdout);
//	for(size_t i = 0; i < aToVector->iSize; ++i)
//		if((i < aBegin) || (i >= aEnd))
//			fputs(". ", stdout);
//		else
//			if(!vector_element_store_file(aToVector->iValues[i], stdout))
//				fputs("Function 'vector_element_store_file()' failed!\n", stderr);
//	fputc('\n', stdout);
	}

void merge_sort(struct TVector *aVector)
	{
	if(vector_size(aVector) < 2)
		return;

	struct TVector work_vector = { 0 };
	if(!vector_attach_sortstats(aVector, &work_vector))	// Create sortstats relationship between `aVector` and `work_vector`
		return;

	if(!vector_clone(aVector, &work_vector))			// Copy all element values from `aVector` to the `work_vector`
		return;

	merge_sort_worker(&work_vector, 0, vector_size(aVector), aVector);

	vector_destroy(&work_vector);
	}
