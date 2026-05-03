#ifndef TSET_HASH_TABLE_OPEN_ADDR_H
#define TSET_HASH_TABLE_OPEN_ADDR_H
/** \file TSetHashTableOpenAddr.h
 *  \brief Implementace typu SetHashTable - (hašovací tabulka využívající otevřené adresování)
 *  \author Petyovský
 *  \version 2026
 *  $Id: TSetHashTableOpenAddr.h 3361 2026-04-20 14:36:03Z petyovsky $
 */

#include <stdbool.h>
#include <stddef.h>
#include "TSetElementHashable.h"

/** \defgroup TSetHashTable 2.1. SetHashTable (hašovací tabulka s otevřeným adresováním)
 *  \brief Definice pomocného datového typu SetHashTable a jeho API (realizace hašovací tabulky využívající otevřeného adresování)
 *  \ingroup TSet
 *  \{
 */

#define HASH_TABLE_INIT_SIZE 256					///< Počáteční počet řádků (přihrádek) hašovací tabulky

/** \brief Deklarace privátního typu SetHashTable
 */
struct TSetHashTable;

/** \brief Inicializace hašovací tabulky
 *  \details Inicializuje složky struktury tak, aby byla výsledkem prázdná hašovací tabulka.
 *  \param[in,out] aTablePtr Ukazatel na místo v paměti, kde má být zapsán ukazatel na inicializovanou hašovací tabulku
 *  \param[in] aCapacity Kapacita přihrádek hašovací tabulky
 *  \return \c true pokud byla hašovací tabulka dané velikosti úspěšně vytvořena.
 */
[[nodiscard]] bool set_hash_table_init(struct TSetHashTable **aTablePtr, size_t aCapacity);

/** \brief Vyhledání elementu o zadané hodnotě v hašovací tabulce využívající otevřené adresování
 *  \details Funkce vracející \c int hodnotu reprezentující výsledek testu, zda hašovací tabulka obsahuje zadanou hodnotu elementu.
 *  \param[in] aTable Ukazatel na existující hašovací tabulku
 *  \param[in] aValue Hodnota elementu hledaná v hašovací tabulce
 *  \param[in,out] aPosPtr Ukazatel na proměnnou určenou pro uložení pozice (indexu) přihrádky s nalezeným elementem \p aValue
 *  \retval -2 Pokud je ukazatel \p aTable neplatný
 *  \retval -1 Pokud je ukazatel \p aPosPtr neplatný
 *  \retval  0 Pokud element byl v hašovací tabulce nalezen (hledaná hodnota elementu je na pozici (indexu) přihrádky \p *aPosPtr)
 *  \retval +1 Pokud element nebyl v hašovací tabulce nalezen (pozice (index) první objevené smazané přihrádky je \p *aPosPtr, pokud nebyla nalezena žádná smazaná přihrádka je v \p *aPosPtr pozice (index) poslední testované přihrádky)
 *  \retval +2 Pokud element nebyl v hašovací tabulce nalezen a hašovací tabulka je zcela zaplněna
 *  \note Funkci implementujte tak, aby podporovala otevřené adresování pro obě varianty: lineárního i kvadratického dohledávání následující pozice v hašovací tabulce.
 *  \note Variantu hledání následující pozice bude možné definovat pomocí uvedených manifestačních konstant preprocesoru `LINEAR_PROBING` a `QUADRATIC_PROBING`.
 */
[[nodiscard]] int set_hash_table_search(const struct TSetHashTable *aTable, TSetElement aValue, size_t *aPosPtr);

/** \brief Vložení elementu do hašovací tabulky
 *  \details Vkládá hodnotu elementu do hašovací tabulky.
 *  \param[in,out] aTable Ukazatel na existující hašovací tabulku
 *  \param[in] aValue Hodnota elementu vkládaná do hašovací tabulky
 *  \return \c true pokud nebyl element s hodnotou \p aValue v původní hašovací tabulce nalezen, následně se podařilo alokací vytvořit kopii elementu a úspěšně tuto kopii vložit do hašovací tabulky
 */
[[nodiscard]] bool set_hash_table_insert(struct TSetHashTable *aTable, TSetElement aValue);

/** \brief Odstranění elementu z hašovací tabulky
 *  \details Odstraní hodnotu elementu z hašovací tabulky.
 *  \param[in,out] aTable Ukazatel na existující hašovací tabulku
 *  \param[in] aValue Hodnota elementu odebíraného z hašovací tabulky
 *  \return \c true pokud byl element s hodnotou \p aValue v hašovací tabulce nalezen, následně se jej podařilo z hašovací tabulky odstranit a paměť přidělenou tomuto elementu dealokovat
 */
[[nodiscard]] bool set_hash_table_erase(struct TSetHashTable *aTable, TSetElement aValue);

/** \brief Deinicializace hašovací tabulky
 *  \details Deinicializuje hašovací tabulku, nastaví všechny přihrádky na smazané.
 *  \param[in,out] aTable Ukazatel na existující hašovací tabulku
 */
void set_hash_table_destroy(struct TSetHashTable *aTable);

/** \brief Výpočet první platné pozice v hašovací tabulce
 *  \details Funkce vrací první pozici prvního platné přihrádky v hašovací tabulce.
 *  \param[in] aTable Ukazatel na existující hašovací tabulku
 *  \return Pozice (index) prvního platné přihrádky v hašovací tabulce
 */
[[nodiscard]] size_t set_hash_table_begin_pos(const struct TSetHashTable *aTable);

/** \brief Zjištění platnosti pozice v hašovací tabulce
 *  \details Funkce (predikát) vracející \c bool hodnotu definující platnost zadaného indexu do hašovací tabulky.
 *  \param[in] aTable Ukazatel na existující hašovací tabulku
 *  \param[in] aPos Pozice (index) přihrádky v hašovací tabulce
 *  \return \c true pokud je pozice platným indexem v hašovací tabulce
 */
[[nodiscard]] bool set_hash_table_is_valid_pos(const struct TSetHashTable *aTable, size_t aPos);

/** \brief Výpočet následující pozice platné přihrádky v hašovací tabulce
 *  \details Výpočet pozice následující platné přihrádky v hašovací tabulce.
 *  \param[in] aTable Ukazatel na existující hašovací tabulku
 *  \param[in] aPos Pozice (index) přihrádky v hašovací tabulce
 *  \return Pozice (index) následující platné přihrádky v hašovací tabulce
 */
[[nodiscard]] size_t set_hash_table_next_pos(const struct TSetHashTable *aTable, size_t aPos);

/** \brief Přečtení hodnoty elementu z platné přihrádky v hašovací tabulce
 *  \details Přečte hodnotu elementu ze zadané pozice přihrádky v hašovací tabulce.
 *  \param[in] aTable Ukazatel na existující hašovací tabulku
 *  \param[in] aPos Pozice (index) přihrádky v hašovací tabulce
 *  \return Hodnota elementu z přihrádky na dané pozici.
 *  \attention Funkce ověřuje platnost ukazatele \b pouze při překladu v režimu `Debug`, kdy pomocí `assert` hlásí běhovou chybu!
 */
[[nodiscard]] TSetElement set_hash_table_value_at_pos(const struct TSetHashTable *aTable, size_t aPos);

/** \} TSetHashTable */

#endif /* TSET_HASH_TABLE_OPEN_ADDR_H */
