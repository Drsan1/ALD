#ifndef ADVANCED_SORTS_H
#define ADVANCED_SORTS_H
/** \file advanced_sorts.h
 *  \brief Deklarace API účinnějších metod třídění
 *  \author Petyovský
 *  \version 2026
 *  $Id: advanced_sorts.h 3317 2026-04-02 17:27:02Z petyovsky $
 */

#include "TVector.h"

/** \defgroup AdvancedSorts 6. Účinnější metody třídění
 *  \brief Deklarace API účinnějších metod třídění
 *  \{
 */

/** \brief Shellovo třídění vkládáním s ubývajícím krokem (Shell Sort) dle Hibbardovy posloupnosti
 *  \details Setřídí vzestupně elementy ve vektoru \p aVector pomocí metody Shellova třídění využívající Hibbardovu posloupnost.
 *  \param[in,out] aVector Ukazatel na tříděný vektor hodnot
 */
void shell_sort(struct TVector *aVector);

/** \brief Rychlé třídění výměnou (Quick Sort) s volbou prostředního elementu jako pivota
 *  \details Setřídí vzestupně elementy ve vektoru \p aVector pomocí metody rychlého třídění výměnou s volbou prostředního elementu jako pivota.
 *  \param[in,out] aVector Ukazatel na tříděný vektor hodnot
 */
void quick_sort(struct TVector *aVector);

/** \} AdvancedSorts */

#endif /* ADVANCED_SORTS_H */
