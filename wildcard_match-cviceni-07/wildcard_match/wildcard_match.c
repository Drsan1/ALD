/** \file wildcard_match.c
 *  \brief Definice funkce wildcard_match() realizující test na shodu řetězců z nichž jeden obsahuje žolíkové znaky
 *  \author Petyovský
 *  \version 2026
 *  $Id: wildcard_match.c 3307 2026-03-30 14:12:07Z petyovsky $
 */
#include "wildcard_match.h"
#include <stdio.h>

bool wildcard_match(const char aWildCardStr[], const char aStr[])
	{
	printf("\twildcard_match(\"%s\",\"%s\")\n", aWildCardStr, aStr);
	if(*aWildCardStr == '\0' && *aStr == '\0')
		return true;
	if(*aWildCardStr == *aStr)
		return wildcard_match(aWildCardStr + 1, aStr + 1);
	if(*aWildCardStr == '?' && *aStr != '\0')
		return wildcard_match(aWildCardStr + 1, aStr + 1);

	if (*aWildCardStr == '*') {
		bool state = true;
		if (*aStr == '\0')
			state = false;
		else if (*aStr != '\0') {
			state = wildcard_match(aWildCardStr, aStr + 1);
		}
		if (!state) {
			state = wildcard_match(aWildCardStr + 1, aStr);
		}
		return state;
	}
	return false;
	}
