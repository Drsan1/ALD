/** \file TSetHashTableOpenAddr.c
 *  \brief Implementace API pro typ SetHashTable - (hašovací tabulka využívající otevřené adresování)
 *  \author Petyovský
 *  \version 2026
 *  $Id: TSetHashTableOpenAddr.c 3361 2026-04-20 14:36:03Z petyovsky $
 */

#define LINEAR_PROBING		1	///< Volba pro lineární dohledávání následující pozice ve funkci: `set_hash_table_search()`
#define QUADRATIC_PROBING	2	///< Volba pro kvadratické dohledávání následující pozice ve funkci: `set_hash_table_search()`

//#define HASH_TABLE_SEARCH_VARIANT	LINEAR_PROBING		///< Varianta využívající lineární dohledávání následující pozice ve funkci: `set_hash_table_search()`
#define HASH_TABLE_SEARCH_VARIANT	QUADRATIC_PROBING	///< Varianta využívající kvadratické dohledávání následující pozice ve funkci: `set_hash_table_search()`

#include <stdlib.h>
#include <assert.h>
#include "TSetHashTableOpenAddr.h"
#include "check.h"

/** \brief Úplná definice typu SetHashTable
 *  \details Typ SetHashTable realizuje hash tabulku s elementy typu ukazatel na SetElementHashable a umožňuje s nimi pracovat pomocí definovaného API.
 */
struct TSetHashTable
	{
	size_t iCapacity;			///< Naalokovaná kapacita vnitřního pole přihrádek
	TSetElement *iBucket[];		///< Pole přihrádek realizující hašovací tabulku (každá přihrádka je ukazatel na typ SetElement Hashable)
	};

/** \brief Funkce vracející hodnotu reprezentující smazanou přihrádku
 *  \details Servisní funkce pro výpočet hodnoty reprezentující značku pro dříve obsazenou, ale nyní již smazanou přihrádku.
 *  \param[in] aTable Ukazatel na existující hašovací tabulku
 *  \return Vrací hodnotu reprezentující značku pro smazanou přihrádku
 *  \attention Funkce ověřuje platnost ukazatele \b pouze při překladu v režimu `Debug`, kdy pomocí `assert` hlásí běhovou chybu!
 */
static inline TSetElement* calc_erased_bucket_marker(const struct TSetHashTable *aTable)
	{
	assert(aTable);
	assert(aTable->iBucket);
	return (TSetElement *) &aTable->iBucket[0];
	}

bool set_hash_table_init(struct TSetHashTable **aTablePtr, size_t aCapacity)
	{
	if(!aTablePtr || !aCapacity)
		return false;

	struct TSetHashTable *new_table =
		calloc(1, offsetof(struct TSetHashTable, iBucket) + aCapacity * sizeof(TSetElement *));

	if(!new_table)
		return false;

	new_table->iCapacity = aCapacity;
	*aTablePtr = new_table;
	return true;
	}

int set_hash_table_search(const struct TSetHashTable *aTable, TSetElement aValue, size_t *aPosPtr)
	{
	if(!aTable)
		return -2;

	if(!aPosPtr)
		return -1;

	assert(aTable->iCapacity);

	/* Zde implementujte algoritmus pro vyhledávání elementu v hašovací tabulce */
	

#if HASH_TABLE_SEARCH_VARIANT == LINEAR_PROBING

	/* Zde implementujte lineární zkoumání následující pozice */
	size_t hash = set_element_hash_value(&aValue);
	size_t pos = hash % aTable->iCapacity;
	size_t firstpos = pos;
	size_t markerpos = 0;
	bool markervalid = false;
	do {
		if (aTable->iBucket[pos] == calc_erased_bucket_marker(aTable))
			{
			if (!markervalid)
				markerpos = pos;
			markervalid = true;
			}
		else if (aTable->iBucket[pos])
			{
			if (set_element_comparator(&aValue, aTable->iBucket[pos]) == 0)
				{ // neni NULL
				*aPosPtr = pos;
				return 0; // je rovno
				}
			}
		else
			{
			*aPosPtr = markervalid ? markerpos : pos;
			return 1; // prazdne misto
			}
		pos = (pos + 1) % aTable->iCapacity;
	} while (pos != firstpos);

#elif HASH_TABLE_SEARCH_VARIANT == QUADRATIC_PROBING

	/* Zde implementujte kvadratické zkoumání následující pozice */
	// h(x) = (c(x) + i^2) % N
	size_t hash = set_element_hash_value(&aValue);
	size_t pos = hash % aTable->iCapacity;
	size_t firstpos = pos;
	size_t markerpos = 0;
	bool markervalid = false;
	size_t i = 0;
	do {
		if (aTable->iBucket[pos] == calc_erased_bucket_marker(aTable))
		{
			if (!markervalid)
				markerpos = pos;
			markervalid = true;
		}
		else if (aTable->iBucket[pos])
		{
			if (set_element_comparator(&aValue, aTable->iBucket[pos]) == 0)
			{ // neni NULL
				*aPosPtr = pos;
				return 0; // je rovno
			}
		}
		else
		{
			*aPosPtr = markervalid ? markerpos : pos;
			return 1; // prazdne misto
		}
		i++;
		pos = (firstpos + i * i) % aTable->iCapacity;
	} while (i < aTable->iCapacity);

#else
	#error Missing correct HASH_TABLE_SEARCH_VARIANT symbol definition
#endif

	return 2;		// Element was not found and hash table is completely full
	}

bool set_hash_table_insert(struct TSetHashTable *aTable, TSetElement aValue)
	{
	/* Zde implementujte vložení kopie elementu do hašovací tabulky na správnou pozici. */
	/* Využijte servisní funkci: set_hash_table_search */
	if (!aTable)
		return false; 
	size_t pos = 0;
	if (set_hash_table_search(aTable, aValue, &pos) != 1)
		return false;
	TSetElement *tmp = malloc(sizeof(TSetElement));
	if (!tmp)
		return false;
	*tmp = aValue;
	aTable->iBucket[pos] = tmp;

	return true;
	}

bool set_hash_table_erase(struct TSetHashTable *aTable, TSetElement aValue)
	{

	/* Zde implementujte odebrání elementu ze správné pozice v hašovací tabulce. */
	/* Využijte servisní funkci: set_hash_table_search */
	// Hash table is full and element was not found, or just element was not found
	if (!aTable)
		return false;
	size_t pos = 0;
	if (set_hash_table_search(aTable, aValue, &pos) != 0)
		return false; 
	free(aTable->iBucket[pos]);
	aTable->iBucket[pos] = calc_erased_bucket_marker(aTable);	
	return true;
	}

void set_hash_table_destroy(struct TSetHashTable *aTable)
	{
	if(!aTable)
		return;

	assert(aTable->iCapacity);
	TSetElement *const Marked_as_erased = calc_erased_bucket_marker(aTable);
	for(size_t i = 0; i < aTable->iCapacity; ++i)
		{
		if(aTable->iBucket[i] != Marked_as_erased)
			free(aTable->iBucket[i]);

		aTable->iBucket[i] = NULL;
		}

	free(aTable);
	}

size_t set_hash_table_begin_pos(const struct TSetHashTable *aTable)
	{
	assert(aTable);
	assert(aTable->iCapacity);

	TSetElement *const Marked_as_erased = calc_erased_bucket_marker(aTable);
	size_t pos = 0;
	for(; pos < aTable->iCapacity; ++pos)
		if( (aTable->iBucket[pos]) && (aTable->iBucket[pos] != Marked_as_erased) )
			break;

	return pos;
	}

bool set_hash_table_is_valid_pos(const struct TSetHashTable *aTable, size_t aPos)
	{
	if(aTable)
		{
		assert(aTable->iCapacity);
		TSetElement *const Marked_as_erased = calc_erased_bucket_marker(aTable);
		return (aPos < aTable->iCapacity) && (aTable->iBucket[aPos]) && (aTable->iBucket[aPos] != Marked_as_erased);
		}

	return false;
	}

size_t set_hash_table_next_pos(const struct TSetHashTable *aTable, size_t aPos)
	{
	assert(aTable);
	assert(aTable->iCapacity);

	TSetElement *const Marked_as_erased = calc_erased_bucket_marker(aTable);
	for(++aPos; aPos < aTable->iCapacity; ++aPos)
		if( (aTable->iBucket[aPos]) && (aTable->iBucket[aPos] != Marked_as_erased) )
			break;

	return aPos;
	}

TSetElement set_hash_table_value_at_pos(const struct TSetHashTable *aTable, size_t aPos)
	{
	assert(aTable);
	assert(aTable->iCapacity);
	assert(aPos < aTable->iCapacity);

	TSetElement *const Marked_as_erased = calc_erased_bucket_marker(aTable);
	assert( (aTable->iBucket[aPos]) && (aTable->iBucket[aPos] != Marked_as_erased) );

	return *aTable->iBucket[aPos];
	}
