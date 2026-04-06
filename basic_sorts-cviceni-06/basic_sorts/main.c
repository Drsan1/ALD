/** \file main.c
 *  \brief Hlavní program testující API přímých metod třídění
 *  \author Petyovský
 *  \version 2026
 *  $Id: main.c 3260 2026-03-16 16:21:51Z petyovsky $
 */

#include <stdio.h>
#include <time.h>
#include "basic_sorts.h"
#include "main_support.h"
#include "check.h"

/** \defgroup MainProgram 6. Hlavní program
 *  \brief Definice funkcí hlavního programu
 *  \{
 */

/** \brief Testování třídicí metody
 *  \details Testovací funkce pro třídicí metodu, funkce změří výpočetní čas, kterou třídicí metoda potřebovala k dokončení setřídění hodnot vektoru. Dále funkce ověří správnost setřídění hodnot a vypíše statistiky o počtu operací použitých pro setřídění.
 *  \param[in] aSortNameStr String reprezentující název třídicí metody
 *  \param[in] aSortFunc Ukazatel na funkci reprezentující třídicí metodu
 *  \param[in] aOrigVector Ukazatel na vstupní vektor
 *  \return \c true pokud testování třídicí metody pro daný vstupní vektor dopadlo úspěšně
 */
bool sort_testing(const char *aSortNameStr, void (*aSortFunc)(struct TVector *), const struct TVector *aOrigVector)
	{
	const bool print_elements_before = false;		// `constexpr` since C23
	const bool print_elements_after = false;		// `constexpr` since C23

	struct TVector vector = { 0 };
	UNLESS(vector_clone(aOrigVector, &vector), goto cleanup_sort_testing);

	printf("\n------ %s ------\n", aSortNameStr);

	if(print_elements_before)
		{
		//vector_for_each(vector_iterator_begin(&vector), print_element);
		//putchar('\n');

		//auto it = vector_for_each_n(vector_iterator_begin(&vector), 20, print_element);		// `auto` since C23
		struct TVectorIterator it = vector_for_each_n(vector_iterator_begin(&vector), 20, print_element);
		puts(vector_iterator_is_valid(&it) ? "..." : "");
		}

	struct timespec start_timespec = { 0, }, end_timespec = { 0, };
	UNLESS(timespec_get(&start_timespec, TIME_UTC));

	(*aSortFunc)(&vector);

	UNLESS(timespec_get(&end_timespec, TIME_UTC));

	if(print_elements_after)
		{
		//vector_for_each(vector_iterator_begin(&vector), print_element);
		//putchar('\n');

		//auto it = vector_for_each_n(vector_iterator_begin(&vector), 20, print_element);		// `auto` since C23
		struct TVectorIterator it = vector_for_each_n(vector_iterator_begin(&vector), 20, print_element);
		puts(vector_iterator_is_valid(&it) ? "..." : "");
		}

	const bool is_sorted = vector_is_ascendantly_sorted(aOrigVector, &vector);
	printf("\nVector is %s\n", is_sorted ? "sorted. OK :-)" : "not sorted!!! FAIL :-(");
	printf("\nSorting time: %lf seconds.\n", difftimespec(end_timespec, start_timespec));

	UNLESS(vector_print_sortstats(&vector));

	vector_destroy(&vector);
	return is_sorted;

cleanup_sort_testing:
	vector_destroy(&vector);
	return false;
	}

//#define VECTOR_INIT_FROM_FILE	"reversed_data.txt"
//#define VECTOR_INIT_FROM_FILE	"select_sort_data.txt"
//#define VECTOR_INIT_FROM_FILE	"bubble_sort_optimized_data.txt"
//#define VECTOR_INIT_FROM_FILE	"shaker_sort_data.txt"
//#define VECTOR_INIT_FROM_FILE	"sorted_data.txt"

int main(int argc, char *argv[])
	{
	srand((unsigned int)time(NULL));

	struct TVector vector_orig = { 0 };

#ifdef VECTOR_INIT_FROM_FILE
	FILE *fsrc = fopen(VECTOR_INIT_FROM_FILE, "r");
	//if(!fsrc)
	//	return 1;
	UNLESS(fsrc, return 1);

	bool initialized = vector_init_file(&vector_orig, fsrc);
	fclose(fsrc);
#else
	bool initialized = vector_init_random(&vector_orig, 10000);
#endif

	//if(!initialized)
	//	return 2;
	UNLESS(initialized, return 2);

#ifdef VECTOR_INIT_FROM_FILE
	printf("Vector of %zu elements from file '" VECTOR_INIT_FROM_FILE "':\n", vector_size(&vector_orig));
#else
	printf("Vector of %zu random elements:\n", vector_size(&vector_orig));
#endif

	sort_testing("Insert Sort", insert_sort, &vector_orig);
	sort_testing("Select Sort", select_sort, &vector_orig);
	sort_testing("Bubble Sort", bubble_sort, &vector_orig);
	sort_testing("Bubble Sort Optimized", bubble_sort_optimized, &vector_orig);
	sort_testing("Bubble Sort Optimized2", bubble_sort_optimized2, &vector_orig);
	sort_testing("Shaker Sort", shaker_sort, &vector_orig);

	vector_destroy(&vector_orig);
	return 0;
	}

/** \} MainProgram */
