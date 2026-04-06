/** \file main.c
 *  \brief Hlavní program (testbench) pro manuální testování správnosti implementace funkce wildcard_match()
 *  \author Petyovský
 *  \version 2026
 *  $Id: main.c 3307 2026-03-30 14:12:07Z petyovsky $
 */

#include <stdio.h>
#include <string.h>
#include "wildcard_match.h"

/** \defgroup MainProgram 2. Hlavní program
 *  \brief Definice funkcí hlavního programu
 *  \{
 */

#define STRINGS_MAXSIZE 1024		///< Maximální velikost vstupních řetězců

int main(int argc, char *argv[])
	{
	printf("Insert wildcard string: ");
	char wildcard_str[STRINGS_MAXSIZE] = { '\0', };
	fgets(wildcard_str, STRINGS_MAXSIZE, stdin);
	wildcard_str[strcspn(wildcard_str, "\r\n")] = '\0';	// `strcspn()` - String Complementary Span: https://en.cppreference.com/w/c/string/byte/strcspn

	char str[STRINGS_MAXSIZE] = { '\0', };
	do
		{
		printf("Insert string: ");
		fgets(str, STRINGS_MAXSIZE, stdin);
		str[strcspn(str, "\r\n")] = '\0';
		if(strpbrk(str, "?*"))							// `strpbrk()` - String Pointer Break: https://en.cppreference.com/w/c/string/byte/strpbrk
			fputs("Warning: input string contains wildcard character(s).\n", stderr);

		if(wildcard_match(wildcard_str, str))
			puts("Strings match passed.\n");
		else
			puts("Strings match failed.\n");
		}
	while(strlen(str));

	return 0;
	}

/** \} MainProgram */
