/** \file basic_sorts.c
 *  \brief Implementace API přímých metod třídění
 *  \author Petyovský
 *  \version 2026
 *  $Id: basic_sorts.c 3260 2026-03-16 16:21:51Z petyovsky $
 */

#include "basic_sorts.h"

void print_element(const struct TVectorIterator *aIter)
	{
	if(!vector_element_store_file(vector_iterator_value(aIter), stdout))
		fputs("Function 'vector_element_store_file()' failed!\n", stderr);
	}

void insert_sort(struct TVector *aVector)
	{
	if (vector_size(aVector) < 2) // nebo !vector_size(aVector)
		return;
	for (size_t i = 1; i < vector_size(aVector); i++) {
		//printf("%zu: ", i);
		TVectorElement tmp = vector_value(aVector, i);
		size_t j = i;
		for (; j > 0; j--) {
			//printf("%zu ", j);
			if (vector_compare_position_value(aVector, j - 1, tmp) < 1) {
				break;
			}
			else {
				vector_move_positions(aVector, j, aVector, j - 1);
			}		
		}
		vector_set_value(aVector, j, tmp);
		//putchar('\n'); 
	}
	/*vector_for_each_n(vector_iterator_begin(aVector), 20, print_element);
	putchar('\n');*/
	}

void select_sort(struct TVector *aVector)
	{
	if (vector_size(aVector) < 2)
		return;
	for (size_t i = 0; i < vector_size(aVector) - 1; i++) {
		//printf("%zu: ", i);
		size_t min = i;
		size_t j = i + 1;
		for (; j < vector_size(aVector); j++) {
			//printf("%zu ", j);
			if(vector_compare_positions(aVector, min, aVector, j) > 0)
				min = j;
		}
		vector_swap_positions(aVector, i, aVector, min);
		//putchar('\n');
	}
	//vector_for_each_n(vector_iterator_begin(aVector), 20, print_element);
	//putchar('\n');
	}

void bubble_sort(struct TVector *aVector)
	{
	if (vector_size(aVector) < 2)
		return;
	size_t i = 0;
	for (; i < vector_size(aVector) - 1; i++) {
		for (size_t j = 0; j < vector_size(aVector) - i - 1; j++) {
			if(vector_compare_positions(aVector, j, aVector, j + 1) > 0)
				vector_swap_positions(aVector, j, aVector, j + 1);
		}
	}
	//vector_for_each_n(vector_iterator_begin(aVector), 20, print_element);
	//putchar('\n');
	}

void bubble_sort_optimized(struct TVector *aVector)
	{
	if (vector_size(aVector) < 2)
		return;
	for (size_t i = 0; i < vector_size(aVector) - 1; i++) {
		bool swapped = false;
		for (size_t j = 0; j < vector_size(aVector) - i - 1; j++) {
			if (vector_compare_positions(aVector, j, aVector, j + 1) > 0) {
				vector_swap_positions(aVector, j, aVector, j + 1);
				swapped = true;
			}
		}
		if (!swapped)
			break;
	}
	//vector_for_each_n(vector_iterator_begin(aVector), 20, print_element);
	//putchar('\n');
	}

void bubble_sort_optimized2(struct TVector *aVector)
	{
	if (vector_size(aVector) < 2)
		return;
	for (size_t i = 0; i < vector_size(aVector) - 1;) {
		size_t newn = 0;
		for (size_t j = 0; j < vector_size(aVector) - 1 - i; j++) {
			if (vector_compare_positions(aVector, j, aVector, j + 1) > 0) {
				vector_swap_positions(aVector, j, aVector, j + 1);
				newn = j;
			}
		}
		i = vector_size(aVector) - newn - 1;
	}
	/*vector_for_each_n(vector_iterator_begin(aVector), 20, print_element);
	putchar('\n');*/
	}

void shaker_sort(struct TVector *aVector) //edit neni moc optimalni 
	{
	if (vector_size(aVector) < 2)
		return;
	size_t left = 0;
	size_t right = vector_size(aVector);
	while(left < right){
		//printf("L: %zu\t R: %zu\n", left, right);
		size_t last = 0;
		for (size_t i = left; i < right - 1; i++) {
			//printf("\ti:%zu\n", i);
			if (vector_compare_positions(aVector, i, aVector, i + 1) > 0) {
				vector_swap_positions(aVector, i, aVector, i + 1);
				last = i;
			}
		}
		right = last + 1;
		last = vector_size(aVector);
		
		for (size_t i = right-1; i > left; i--){
			//printf("\t\ti:%zu\n", i);
			if (vector_compare_positions(aVector, i - 1, aVector, i) > 0) {
				vector_swap_positions(aVector, i - 1, aVector, i);
				last = i;
			 }
		}
		left = last;
	}
			
	/*vector_for_each_n(vector_iterator_begin(aVector), 20, print_element);
	putchar('\n');*/
	}
