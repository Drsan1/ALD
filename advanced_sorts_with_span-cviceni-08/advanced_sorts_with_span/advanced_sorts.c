/** \file advanced_sorts.c
 *  \brief Implementace API účinnějších metod třídění
 *  \author Petyovský
 *  \version 2026
 *  $Id: advanced_sorts.c 3317 2026-04-02 17:27:02Z petyovsky $
 */

#include "advanced_sorts.h"

 /** \brief Formátovaný tisk hodnoty elementu za předpokladu platné hodnoty predikátu
  *  \details Vytiskne formátovaně hodnotu elementu pokud je parametr \p aCondition platný, jinak se vytisknou zástupné znaky.
  *  \param[in] aCondition pokud je \c true bude hodnota vytisknuta, jinak se vytisknou zástupné znaky
  *  \param[in] aValue Hodnota elementu k vytisknutí
  */
[[maybe_unused]] static void vector_element_print_if(bool aCondition, TVectorElement aValue)
{
	if (!aCondition)
		fputs(". ", stdout);
	else
		if (!vector_element_store_file(aValue, stdout))
			fputs("Function 'vector_element_store_file()' failed!\n", stderr);
}

void shell_sort(struct TVector* aVector)
{
	if (vector_size(aVector) < 2)
		return;
	size_t h = 1;
	while (h <= aVector->iSize / 2) {
		h = (h << 1) | 1;
	}
	h >>= 1;
	//printf("h: %zu\n", h);
	while (h > 0) {
		for (size_t k = 0; k < h; k++) {
			for (size_t i = k + h; i < aVector->iSize; i += h) {
				TVectorElement tmp = vector_value(aVector, i);
				size_t j = i;
				for (; j > k; j -= h) {
					if (vector_compare_position_value(aVector, j - h, tmp) <= 0) {
						break;
					}
					else {
						vector_move_positions(aVector, j, aVector, j - h);
					}
				}
				vector_set_value(aVector, j, tmp);
			}
		}
		h >>= 1;
	}
	/*for(size_t x = 0; x < aVector->iSize; ++x)
		vector_element_store_file(aVector->iValues[x], stdout);
	putchar('\n');

	for (size_t x = 0; x < aVector->iSize; ++x)
			vector_element_print_if((x % h == k) && (x <= i), aVector->iValues[x]);
		putchar('\n');
	*/
}

/** \brief Vnitřní a lokální funkce realizující rekurzivní pracovní funkci pro metodu (Quick Sort)
 *  \details Realizuje rekurzivní pracovní funkci pro třídicí metodu (Quick Sort) pro elementy vektoru v rozsahu pozic `<k,l>`.
 *  \param[in,out] aVector Ukazatel na tříděný vektor hodnot
 *  \param[in] k Index levého elementu z rozsahu `<k,l>`
 *  \param[in] l Index pravého elementu z rozsahu `<k,l>`
 */
static void quick_sort_worker(struct TVector* /*restrict*/ aVector, size_t k, size_t l)
{
	/*for(size_t x = 0; x < aVector->iSize; ++x)
		vector_element_print_if( (x >= k) && (x <= l), aVector->iValues[x]);
	putchar('\n');*/
	size_t s = (k + l) / 2;
	TVectorElement pvt = vector_value(aVector, s); //pivot
	size_t i = k, j = l;
	do {
		while (vector_compare_position_value(aVector, i, pvt) == -1)
			i++;
		while (vector_compare_position_value(aVector, j, pvt) == 1)
			j--;
		if (i > j)
			break;
		vector_swap_positions(aVector, i, aVector, j);
		i++;
		if (j > k)
			j--;
	} while (i <= j);
	//printf("i = %zu\tj = %zu\n", i, j);
	if (j > k)
		quick_sort_worker(aVector, k, j);
	if (i < l)
		quick_sort_worker(aVector, i, l);
}

void quick_sort(struct TVector* aVector)
{
	if (vector_size(aVector) < 2)
		return;

	quick_sort_worker(aVector, 0, aVector->iSize - 1);
}
