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

void print_element(const struct TVectorIterator *aIter)
	{
	printf(TVECTOR_ELEMENT_FRMSTR " ", vector_iterator_value(aIter));
	//vector_element_store_file(vector_iterator_value(aIter), stdout);
	}

int main(int argc, char *argv[])
	{
	if (argc != 3)
		return 5;
	struct TVector u = { 0 };
	struct TVector v = { 0 };

	FILE* fr1 = fopen(argv[1], "r");
	if (!fr1)
		return 1;
	UNLESS(vector_init_file(&u, fr1), goto clean_up);

	FILE* fr2 = fopen(argv[2], "r");
	if (!fr2)
		return 2;
	UNLESS(vector_init_file(&v, fr2), goto clean_up);

	vector_for_each(vector_iterator_begin(&u), print_element);
	putchar('\n');

	vector_for_each(vector_iterator_begin(&v), print_element);
	putchar('\n');

	int state = vector_threeway_comparator(&u, &v);
	switch (state) {
	case 0:
		printf("Jsou si rovny\n");
		break;
	case 1:
		printf("u > v\n");
		break;
	case -1:
		printf("u < v\n");
		break;
	default:
		printf("Neznámý stav!\n");
		break;
	}

	fclose(fr1);
	fclose(fr2);
clean_up:
	vector_destroy(&u);
	vector_destroy(&v);
	return 0;
	}

/** \} MainProgram */
