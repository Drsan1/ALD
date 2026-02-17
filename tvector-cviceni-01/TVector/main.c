/** \file main.c
 *  \brief Hlavní program testující Basic Vector API
 *  \author Petyovský
 *  \version 2026
 *  $Id: main.c 3192 2026-02-08 18:45:10Z petyovsky $
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "TVector.h"
#include "main_support.h"
#include "check.h"

/** \defgroup MainProgram 3. Hlavní program
 *  \brief Definice funkcí hlavního programu
 *  \{
 */

/** \brief Vytisknutí elementu vektoru pomocí iterátoru
 *  \details Tisk hodnoty elementu vektoru, na který ukazuje iterátor.
 *  \param[in] aIter Ukazatel na existující iterátor
 */
void print_element(const struct TVectorIterator *aIter)
	{
	printf(TVECTOR_ELEMENT_FRMSTR " ", vector_iterator_value(aIter));
	//vector_element_store_file(vector_iterator_value(aIter), stdout);
	}

/** \brief Testování funkce `vector_resize()`
 *  \details Testovací funkce pro `vector_resize()`, funkce změří výpočetní čas a velikost paměti, kterou funkce potřebovala k dokončení změny velikosti vektoru.
 *  \param[in] aOrigVector Ukazatel na vstupní vektor
 *  \param[in] aNewSize Nově požadovaná velikost vektoru
 */
void resize_testing(const struct TVector *aOrigVector, size_t aNewSize)
	{
	struct TVector vector = { 0 };
	UNLESS(vector_clone(aOrigVector, &vector), goto cleanup_resize_testing);

	//for(size_t i = 0; i < vector_size(&vector); ++i)
	//	printf(TVECTOR_ELEMENT_FRMSTR " ", vector_value(&vector, i));
	//putchar('\n');

	printf("\n------ Resize to %zu ------\n", aNewSize);
	//vector_for_each(vector_iterator_begin(&vector), print_element);
	//putchar('\n');

	struct timespec start_timespec = { 0, }, end_timespec = { 0, };
	UNLESS(timespec_get(&start_timespec, TIME_UTC));

	UNLESS(vector_resize(&vector, aNewSize), goto cleanup_resize_testing);

	UNLESS(timespec_get(&end_timespec, TIME_UTC));

	//vector_for_n(vector_iterator_begin(&vector), 20, print_element);
	//putchar('\n');

	printf("\nResizing time: %lf seconds.\n", difftimespec(end_timespec, start_timespec));
	printf("Allocation size max peak: %zu.\n", get_alloc_max_peak_size());

cleanup_resize_testing:
	vector_destroy(&vector);
	}

//#define VECTOR_INIT_FROM_FILE	"reversed_data.txt" //edit comment

int main(int argc, char *argv[])
	{
	srand((unsigned int)time(NULL));

	struct TVector vector1 = { 0 };

#ifdef VECTOR_INIT_FROM_FILE
	FILE *fsrc = fopen(VECTOR_INIT_FROM_FILE, "r");
	//if(!fsrc)
	//	return 1;
	UNLESS(fsrc, return 1);

	bool initialized = vector_init_file(&vector1, fsrc);
	fclose(fsrc);
#else
	bool initialized = vector_init_random(&vector1, 5); //edit 50000
	//bool initialized = vector_init(&vector1, 5); //edit new
#endif

	//if(!initialized)
	//	return 2;
	UNLESS(initialized, return 2);

#ifdef VECTOR_INIT_FROM_FILE
	printf("Vector of %zu elements from file '" VECTOR_INIT_FROM_FILE "':\n", vector_size(&vector1));
#else
	printf("Vector of %zu random elements:\n", vector_size(&vector1));
#endif

	//printf("vector1 is empty: %s\n", vector_is_empty(&vector1) ? "true" : "false");
	printf("vector1 is empty: %s\n", bool_to_cstring(vector_is_empty(&vector1)) );

	// vector_set_value(&vector1, 0, 15); //edit testovani 

	#ifndef NDEBUG
		// Debug configuration (vector API value access)
		for(size_t i = 0; i < vector_size(&vector1); ++i)
			printf(TVECTOR_ELEMENT_FRMSTR " ", vector_value(&vector1, i));
	#else
		// Release configuration (direct value access)
		if(vector1.iValues)
			{
			for(size_t i = 0; i < vector1.iSize; ++i)
				printf(TVECTOR_ELEMENT_FRMSTR " ", vector1.iValues[i]);
			}
	#endif /* NDEBUG */
	putchar('\n');

	//vector_for_each(vector_iterator_begin(&vector1), print_element);
	//putchar('\n');

	resize_testing(&vector1, 10);

	//vector_for_each(vector_iterator_begin(&vector1), print_element);
	//putchar('\n');

	//resize_testing(&vector1, 1000000);		// 4Mb //edit comment
	//resize_testing(&vector1, 10000000);		// 40Mb
	//resize_testing(&vector1, 100000000);	// 400Mb
	//resize_testing(&vector1, 1000000000);	// 4000Mb

	//vector_for_n(vector_iterator_begin(&vector1), 20, print_element);
	//putchar('\n');

	vector_destroy(&vector1);
	return 0;
	}

/** \} MainProgram */
