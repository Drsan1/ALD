/** \file main.c
 *  \brief Hlavní program pro testování ADT TSet - (realizace množiny pomocí nesetříděného pole)
 *  \author Petyovský
 *  \version 2026
 *  $Id: main.c 3361 2026-04-20 14:36:03Z petyovsky $
 */

#include <stdio.h>
#include "testbench.h"
#include "TSet.h"
#include "main_support.h"

/** \defgroup MainProgram 5. Hlavní program
 *  \brief Definice funkcí hlavního programu
 *  \{
 */

const struct TTestPlan testplan[] =
	{
	//#include "testplan_set1.h"
	#include "testplan_set2.h"
	};

bool testset2_hardcoded(void)
	{
	struct TSet set2 = { 0 };
	set_init(&set2);
	gSetElementComparisons = 0;

	printf("set2 is empty: %s\n", bool_to_cstring(set_is_empty(&set2)) );
	UNLESS(set_insert(&set2, 222), goto cleanup_testset2_hardcoded);
	printf("set2 is empty: %s\n", bool_to_cstring(set_is_empty(&set2)) );

	for(struct TSetIterator it = set_iterator_begin(&set2); set_iterator_is_valid(&it); set_iterator_to_next(&it))
		printf("%" TSET_ELEMENT_FRMSTR " ", set_iterator_value(&it));
	putchar('\n');

	UNLESS(set_insert(&set2, 42), goto cleanup_testset2_hardcoded);
	UNLESS(set_insert(&set2, 43), goto cleanup_testset2_hardcoded);
	UNLESS(set_insert(&set2, 40), goto cleanup_testset2_hardcoded);
	UNLESS(set_insert(&set2, 41), goto cleanup_testset2_hardcoded);
	UNLESS(set_insert(&set2, 333), goto cleanup_testset2_hardcoded);

	set_for_each(set_iterator_begin(&set2), print_element);
	putchar('\n');

	UNLESS(set_erase(&set2, 222), goto cleanup_testset2_hardcoded);
	UNLESS(set_erase(&set2, 40), goto cleanup_testset2_hardcoded);

	set_for_each(set_iterator_begin(&set2), print_element);
	putchar('\n');

	const bool contains = set_is_element_of(&set2, 42);
	printf("42 is element of set2: %s\n", bool_to_cstring(contains));

	//const struct TSetIterator it = set_find(&set2, 42);
	//printf("set2 find 42: %s\n", bool_to_cstring(set_iterator_is_valid(&it) && set_iterator_value(&it) == 42) );

	set_destroy(&set2);

	printf("\nTSetElement comparisons: %zu\n", gSetElementComparisons);
	return true;

cleanup_testset2_hardcoded:
	set_destroy(&set2);
	return false;
	}

int main(int argc, char *argv[])
	{
	struct TSet set1 = { 0 };
	set_init(&set1);

	UNLESS(testbench_run(&set1, ITEMS_OF(testplan), testplan),
		goto cleanup_main_testbench_run_set1);

	//if(!testbench_run(&set1, ITEMS_OF(testplan), testplan))
	//	{
	//	puts("testplan failed!!!");
	//	set_destroy(&set1);
	//	return 1;
	//	}

	set_destroy(&set1);

	testset2_hardcoded();
	return 0;

cleanup_main_testbench_run_set1:
	set_destroy(&set1);
	return 1;
	}

/** \example cv11_reports_TSet_unsorted_dynamic_array_set1.lst
 * Příklad výstupu programu pro soubor `testplan_set1.h`.
 */

/** \example cv11_reports_TSet_unsorted_dynamic_array_set2.lst
 * Příklad výstupu programu pro soubor `testplan_set2.h`.
 */

/** \} MainProgram */
