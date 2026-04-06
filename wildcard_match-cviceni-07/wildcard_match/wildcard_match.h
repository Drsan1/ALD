#ifndef WILDCARD_MATCH_H
#define WILDCARD_MATCH_H
/** \file wildcard_match.h
 *  \brief Deklarace funkce wildcard_match() realizující test na shodu řetězců z nichž jeden obsahuje žolíkové znaky
 *  \author Petyovský
 *  \version 2026
 *  $Id: wildcard_match.h 3307 2026-03-30 14:12:07Z petyovsky $
 */

#include <stdbool.h>

/** \defgroup Wildcard_matching 1. Wildcard matching
 *  \brief Implementace funkce realizující test shody dvou řetězců z nichž jeden obsahuje žolíkové znaky
 *  \{
 */

/** \brief Implementace funkce pro ověření shody řetězců z nichž jeden obsahuje žolíkové znaky
 *  \details Funkce realizující rekurzivní řešení úlohy ověření shody dvou řetězců z nichž jeden obsahuje žolíkové znaky: `*` a `?`.
 *  \param[in] aWildCardStr Vstupní řetězec se vzorem, který může obsahovat zástupné (tzv. žolíkové) znaky
 *  \param[in] aStr Vstupní řetězec reprezentující obecný řetězec
 *  \return \c true pokud test na shodu vzoru a řetězce dopadl úspěšně
 */
[[nodiscard]] bool wildcard_match(const char aWildCardStr[], const char aStr[]);

/** \} Wildcard_matching */

#endif /* WILDCARD_MATCH_H */
