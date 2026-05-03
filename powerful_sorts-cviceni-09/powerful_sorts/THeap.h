#ifndef THEAP_H
#define THEAP_H
/** \file THeap.h
 *  \brief Definice typu Heap pro přístup k elementům vektoru jako k uzlům haldy (vyžaduje Sortable Vector with shareable sortstats)
 *  \author Petyovský
 *  \version 2026
 *  $Id: THeap.h 3328 2026-04-10 12:15:33Z petyovsky $
 */

#include <assert.h>
#include "TVector.h"

/** \defgroup THeap 2.2.2. Datový typ Heap pro přístup k elementům vektoru jako k uzlům haldy
 *  \brief Definice typu Heap, který umožňuje přístup k elementům vektoru jako k uzlům haldy (vyžaduje Sortable Vector s vlastností sdílení statistik třídění)
 *  \ingroup sortable_TVector_with_shareable_sortstats
 *  \{
 */
typedef TVectorElement THeapElement;		///< Definice typu HeapElement jako alias na typ VectorElement
#define THEAP_ELEMENT_FRMSTR TVECTOR_ELEMENT_FRMSTR	///< Definice formátovacího řetězce (ovšem bez znaku `%`) určeného pro souborové operace s typem HeapElement

/** \brief Definice typu Heap
 *  \details Typ Heap zajišťuje přístup k elementům vektoru, jako by šlo o uzly haldy a umožňuje pracovat s hodnotami v uzlech pomocí definovaného API.
 */
struct THeap
	{
	struct TVector iVector;					///< Reprezentuje existující vektor (jako haldu se zkrácenou délkou)
	size_t iOriginalSize;					///< Původní délka vektoru (při založení haldy)
	};

/** \brief Definice typu HeapIterator
 *  \details HeapIterator se při vzniku naváže na zvolenou haldu a následně umožňuje přistupovat k jejím uzlům pomocí definovaného API.
 *  \invariant Obecně platný a testovatelný předpoklad: `(iVector->iValues == NULL && iVector->iSize == 0 && iOriginalSize == 0)` nebo `(iVector->iValues != NULL && iVector->iSize != 0 && iOriginalSize >= iVector->iSize)`.
 */
struct THeapIterator
	{
	struct TVectorIterator iVectorIter;		///< Reprezentuje iterátor na uzly haldy (tj. vektor se zkrácenou délkou)
	};

/** \brief Vytvoření Heap pro celý vektor
 *  \details Vytvoří novou hodnotu typu Heap s rozsahem pro přístup k elementům celého vektoru.
 *  \param[in] aVector Existující vektor
 *  \return Nový Heap umožňující přístup ke všem elementům vektoru \p aVector
 */
[[nodiscard]] static inline struct THeap heap_create(struct TVector aVector)
	{
	return (struct THeap) { .iVector = aVector, .iOriginalSize = aVector.iSize };
	}

/** \brief Vytvoření Heap pro část vektoru od počátku a definované délce
 *  \details Vytvoří novou hodnotu typu Heap s rozsahem pro přístup k elementům vektoru od počátku až do definované délky.
 *  \param[in] aVector Existující vektor
 *  \param[in] aSize Délka rozsahu od počátku existujícího vektoru (Délka = poslední pozice + 1)
 *  \return Nový Heap umožňující přístup k elementům vektoru \p aVector od počátku až do definované délky
 */
[[nodiscard]] static inline struct THeap heap_create_left(struct TVector aVector, size_t aSize)
	{
	size_t original_size = aVector.iSize;
	if(aVector.iSize < aSize || !aSize)
		{
		aVector.iValues = NULL;
		aVector.iSize = original_size = 0;
		}
	else
		aVector.iSize = aSize;

	return (struct THeap) { .iVector = aVector, .iOriginalSize = original_size };
	}

/** \brief Zjištění, zda je halda prázdná
 *  \details Funkce (predikát) vracející \c bool hodnotu reprezentující test, zda má halda nulovou velikost.
 *  \param[in] aHeap Ukazatel na existující haldu
 *  \return \c true pokud je halda prázdná
 */
[[nodiscard]] static inline bool heap_is_empty(const struct THeap *aHeap)
	{
	assert(aHeap);
	return !aHeap->iVector.iSize;
	//return vector_is_empty(&aHeap->iVector);
	}

/** \brief Přečtení počtu uzlů haldy
 *  \details Vrací velikost (počet uzlů) haldy.
 *  \param[in] aHeap Ukazatel na existující haldu
 *  \return Počet uzlů haldy nebo hodnota 0 v případě prázdné haldy
 *  \attention Funkce ověřuje platnost ukazatele \p aHeap \b pouze při překladu v režimu `Debug`, kdy pomocí `assert` hlásí běhovou chybu!
 */
[[nodiscard]] static inline size_t heap_size(const struct THeap *aHeap)
	{
	assert(aHeap);
	return aHeap->iVector.iSize;
	//return vector_size(&aHeap->iVector);
	}

/** \brief Dekrementace počtu uzlů haldy
 *  \details Sníží o 1 počet uzlů haldy a vrací tuto novou velikost.
 *  \param[in,out] aHeap Ukazatel na existující haldu
 *  \return Počet uzlů haldy snížený o 1
 *  \attention Funkce ověřuje platnost ukazatele \p aHeap a skutečnost, že halda po dekrementaci obsahuje stále alespoň jeden uzel. Oba tyto testy jsou provedeny \b pouze při překladu v režimu `Debug`, kdy pomocí `assert` hlásí běhovou chybu!
 */
static inline size_t heap_decrement_size(struct THeap *aHeap)
	{
	assert(aHeap);
	assert(aHeap->iVector.iSize > 1);
	return --aHeap->iVector.iSize;
	}

/** \brief Inkrementace počtu uzlů haldy
 *  \details Zvýší o 1 počet uzlů haldy a vrací tuto novou velikost.
 *  \param[in,out] aHeap Ukazatel na existující haldu
 *  \return Počet uzlů haldy zvýšený o 1
 *  \attention Funkce ověřuje platnost ukazatele \p aHeap a skutečnost, že halda po dekrementaci obsahuje stále alespoň jeden uzel. Oba tyto testy jsou provedeny \b pouze při překladu v režimu `Debug`, kdy pomocí `assert` hlásí běhovou chybu!
 */
static inline size_t heap_increment_size(struct THeap *aHeap)
	{
	assert(aHeap);
	assert(aHeap->iVector.iSize < aHeap->iOriginalSize);
	return ++aHeap->iVector.iSize;
	}

/** \brief Obnovení původního počtu uzlů haldy
 *  \details Obnoví původní počet uzlů haldy získaný při vytvoření haldy z počtu elementů vektoru.
 *  \param[in,out] aHeap Ukazatel na existující haldu
 *  \return Původní počet uzlů haldy získaný z vektoru při vytvoření haldy
 *  \attention Funkce ověřuje platnost ukazatele \p aHeap \b pouze při překladu v režimu `Debug`, kdy pomocí `assert` hlásí běhovou chybu!
 */
static inline size_t heap_restore_size(struct THeap *aHeap)
	{
	assert(aHeap);
	return aHeap->iVector.iSize = aHeap->iOriginalSize;
	}

/** \brief Přečtení hodnoty z uzlu haldy dle požadované pozice
 *  \details Vrací hodnotu z požadovaného uzlu (indexu) haldy.
 *  \param[in] aHeap Ukazatel na existující haldu
 *  \param[in] aPos Požadovaná pozice (index) uzlu haldy
 *  \return Hodnota uložená v uzlu haldy \p aHeap na pozici uzlu \p aPos
 *  \attention Funkce ověřuje platnost ukazatele a rozsah parametru \p aPos \b pouze při překladu v režimu `Debug`, kdy pomocí `assert` hlásí běhovou chybu!
 */
[[nodiscard]] static inline THeapElement heap_value(const struct THeap *aHeap, size_t aPos)
	{
	return vector_value(&aHeap->iVector, aPos);
	}

/** \brief Zapsání hodnoty do uzlu haldy na požadovanou pozici
 *  \details Zápis nové hodnoty do uzlu haldy na požadovanou pozici (index).
 *  \param[in,out] aHeap Ukazatel na existující haldu
 *  \param[in] aPos Požadovaná pozice (index) uzlu haldy
 *  \param[in] aValue Nová hodnota uzlu haldy
 *  \attention Funkce ověřuje platnost ukazatele a rozsah parametru \p aPos \b pouze při překladu v režimu `Debug`, kdy pomocí `assert` hlásí běhovou chybu!
 */
static inline void heap_set_value(struct THeap *aHeap, size_t aPos, THeapElement aValue)
	{
	vector_set_value(&aHeap->iVector, aPos, aValue);
	}

/** \brief Přesun hodnot mezi uzly haldy
 *  \details Přesun hodnoty z/do libovolného uzlu haldy dle pozice nebo případně i mezi různými haldami. Parametry \p aToHeap a \p aFromHeap mohou být stejné.
 *  \param[in,out] aToHeap Ukazatel na cílovou haldu
 *  \param[in] aToPos Pozice (index) cílového uzlu v haldě \p aToHeap
 *  \param[in] aFromHeap Ukazatel na zdrojovou haldu
 *  \param[in] aFromPos Pozice (index) zdrojového uzlu v haldě \p aFromHeap
 *  \attention Funkce ověřuje platnost ukazatelů a rozsah indexů \b pouze při překladu v režimu `Debug`, kdy pomocí `assert` hlásí běhovou chybu!
 */
static inline void heap_move_positions(struct THeap *aToHeap, size_t aToPos, const struct THeap *aFromHeap, size_t aFromPos)
	{
	vector_move_positions(&aToHeap->iVector, aToPos, &aFromHeap->iVector, aFromPos);
	}

/** \brief Porovnání hodnot uzlů haldy
 *  \details Trojcestně porovná hodnoty mezi zvolenými uzly jedné haldy nebo případně i mezi různými haldami. Parametry \p aLeftHeap a \p aRightHeap mohou být stejné.
 *  \param[in] aLeftHeap Ukazatel na levou haldu při porovnání
 *  \param[in] aLeftPos Pozice (index) (LHS) uzlu v haldě \p aLeftHeap
 *  \param[in] aRightHeap Ukazatel na pravou haldu při porovnání
 *  \param[in] aRightPos Pozice (index) (RHS) uzlu v haldě \p aRightHeap
 *  \retval -1 Pokud (LHS < RHS)
 *  \retval  0 Pokud (LHS = RHS)
 *  \retval +1 Pokud (LHS > RHS)
 *  \attention Funkce ověřuje platnost ukazatelů a rozsah indexů \b pouze při překladu v režimu `Debug`, kdy pomocí `assert` hlásí běhovou chybu!
 */
[[nodiscard]] static inline int heap_compare_positions(const struct THeap *aLeftHeap, size_t aLeftPos, const struct THeap *aRightHeap, size_t aRightPos)
	{
	return vector_compare_positions(&aLeftHeap->iVector, aLeftPos, &aRightHeap->iVector, aRightPos);
	}

/** \brief Porovnání mezi hodnotou v uzlu haldy a hodnotou typu HeapElement
 *  \details Trojcestně porovná hodnotu ve zvoleném uzlu haldy se zadanou hodnotou \p aValue.
 *  \param[in] aHeap Ukazatel na existující haldu pro porovnání
 *  \param[in] aPos Pozice (index) (LHS) uzlu v haldě \p aHeap
 *  \param[in] aValue Hodnota stojící při porovnání na pravé straně (RHS)
 *  \retval -1 Pokud (LHS < RHS)
 *  \retval  0 Pokud (LHS = RHS)
 *  \retval +1 Pokud (LHS > RHS)
 *  \attention Funkce ověřuje platnost ukazatele a rozsah indexu vektoru \b pouze při překladu v režimu `Debug`, kdy pomocí `assert` hlásí běhovou chybu!
 */
[[nodiscard]] static inline int heap_compare_position_value(const struct THeap *aHeap, size_t aPos, THeapElement aValue)
	{
	return vector_compare_position_value(&aHeap->iVector, aPos, aValue);
	}

/** \brief Vzájemná výměna hodnot mezi uzly haldy
 *  \details Vzájemně zamění hodnoty mezi zvolenými uzly haldy dle pozice nebo případně i mezi různými haldami. Parametry \p aLeftHeap a \p aRightHeap mohou být stejné.
 *  \param[in,out] aLeftHeap Ukazatel na levou haldu pro záměnu hodnoty uzlu
 *  \param[in] aLeftPos Pozice (index) (LHS) uzlu v haldě \p aLeftHeap
 *  \param[in,out] aRightHeap Ukazatel na pravou haldu pro záměnu hodnoty uzlu
 *  \param[in] aRightPos Pozice (index) (RHS) uzlu v haldě \p aRightHeap
 *  \attention Funkce ověřuje platnost ukazatelů a rozsah indexů \b pouze při překladu v režimu `Debug`, kdy pomocí `assert` hlásí běhovou chybu!
 */
static inline void heap_swap_positions(struct THeap *aLeftHeap, size_t aLeftPos, struct THeap *aRightHeap, size_t aRightPos)
	{
	vector_swap_positions(&aLeftHeap->iVector, aLeftPos, &aRightHeap->iVector, aRightPos);
	}

/** \brief Vzájemná výměna hodnot mezi uzlem haldy a proměnnou typu HeapElement
 *  \details Vzájemně zamění hodnotu v uzlu haldy na zvolené pozici se zadanou hodnotou v proměnné typu HeapElement.
 *  \param[in,out] aHeap Ukazatel na haldu s uzlem určeným pro záměnu hodnoty
 *  \param[in] aPos Pozice (index) uzlu haldy \p aHeap
 *  \param[in,out] aValue Ukazatel na hodnotu proměnné typu HeapElement pro záměnu
 *  \attention Funkce ověřuje platnost obou ukazatelů a rozsah indexu vektoru \b pouze při překladu v režimu `Debug`, kdy pomocí `assert` hlásí běhovou chybu!
 */
static inline void heap_swap_position_value(struct THeap *aHeap, size_t aPos, THeapElement *aValue)
	{
	vector_swap_position_value(&aHeap->iVector, aPos, aValue);
	}

/** \brief Vypočte pozici uzlu levého následníka
 *  \details Vypočte pozici uzlu představující levého následníka uzlu na pozici předané jako parametr
 *  \param[in] aPos Pozice (index) rodičovského uzlu
 *  \return Pozice uzlu představující levého následníka uzlu na pozici \p aPos
 */
[[nodiscard]] static inline size_t heap_pos_to_left(size_t aPos)
	{
	return aPos + aPos + 1;
	}

/** \brief Vypočte pozici uzlu pravého následníka
 *  \details Vypočte pozici uzlu představující pravého následníka uzlu na pozici předané jako parametr
 *  \param[in] aPos Pozice (index) rodičovského uzlu
 *  \return Pozice uzlu představující pravého následníka uzlu na pozici \p aPos
 */
[[nodiscard]] static inline size_t heap_pos_to_right(size_t aPos)
	{
	return aPos + aPos + 2;
	}

/** \brief Vypočte pozici rodičovského uzlu
 *  \details Vypočte pozici uzlu představujícího rodičovský uzel k pozici uzlu předané jako parametr
 *  \param[in] aPos Pozice (index) levého nebo pravého uzlu
 *  \return Pozice uzlu představující rodičovský uzel k pozici uzlu \p aPos
 *  \attention Funkce ověřuje nenulový index \b pouze při překladu v režimu `Debug`, kdy pomocí `assert` hlásí běhovou chybu!
 */
[[nodiscard]] static inline size_t heap_pos_to_parent(size_t aPos)
	{
	assert(aPos);
	return (aPos - 1) / 2;
	}

/** \brief Vytvoření nového iterátoru ukazujícího na kořenový uzel haldy
 *  \details Vytvoří a vrací nový iterátor, který je navázán (asociován) na zadanou haldu a ukazuje na její kořenový uzel.
 *  \param[in] aHeap Ukazatel na existující haldu
 *  \return Nový iterátor asociovaný na \p aHeap ukazující na kořenový uzel haldy
 */
[[nodiscard]] static inline struct THeapIterator heap_iterator_begin(const struct THeap *aHeap)
	{
	return (struct THeapIterator) { vector_iterator_begin(&aHeap->iVector) };
	}

/** \brief Vytvoření nového iterátoru ukazujícího na nejpravější listový uzel haldy
 *  \details Vytvoří a vrací nový iterátor, který je navázán (asociován) na zadanou hladu a ukazuje na její nejpravější listový (tj. poslední) uzel.
 *  \param[in] aHeap Ukazatel na existující haldu
 *  \return Nový iterátor asociovaný na \p aHeap ukazující na její nejpravější listový uzel
 */
[[nodiscard]] static inline struct THeapIterator heap_iterator_last(const struct THeap *aHeap)
	{
	return (struct THeapIterator) { vector_iterator_last(&aHeap->iVector) };
	}

/** \brief Zjištění platnosti iterátoru
 *  \details Funkce (predikát) vracející \c bool hodnotu definující platnost iterátoru.
 *  \param[in] aIter Ukazatel na existující iterátor
 *  \return \c true pokud je iterátor platný a ukazuje na platné místo v asociované haldě
 */
[[nodiscard]] static inline bool heap_iterator_is_valid(const struct THeapIterator *aIter)
	{
	return vector_iterator_is_valid(&aIter->iVectorIter);
	}

/** \brief Posunutí iterátoru vpřed
 *  \details Funkce ověří platnost iterátoru, a pokud je platný, zajistí jeho posun vpřed (tj. na uzel napravo ve stejné vrstvě, případně na nejlevější uzel v následující vrstvě) v asociované haldě.
 *  \param[in,out] aIter Ukazatel na existující iterátor
 *  \return \c true pokud je iterátor platný a ukazuje i po posunutí na platné místo v asociované haldě
 */
static inline bool heap_iterator_to_next(struct THeapIterator *aIter)
	{
	return vector_iterator_to_next(&aIter->iVectorIter);
	}

/** \brief Posunutí iterátoru vzad
 *  \details Funkce ověří platnost iterátoru, a pokud je platný, zajistí jeho posun vzad (tj. na uzel nalevo ve stejné vrstvě, případně na nejpravější uzel v předchozí vrstvě) v asociované haldě.
 *  \param[in,out] aIter Ukazatel na existující iterátor
 *  \return \c true pokud je iterátor platný a ukazuje i po posunutí na platné místo v asociované haldě
 */
static inline bool heap_iterator_to_prev(struct THeapIterator *aIter)
	{
	return vector_iterator_to_prev(&aIter->iVectorIter);
	}

/** \brief Posunutí iterátoru na levého následníka
 *  \details Funkce ověří platnost iterátoru, a pokud je platný, posune ho na levého následníka (tj. na uzel v další vrstvě představující levého následníka uzlu, na který směřoval iterátor \p aIter před zavoláním této funkce).
 *  \param[in,out] aIter Ukazatel na existující iterátor
 *  \return \c true pokud je iterátor platný a ukazuje i po posunutí na platné místo v asociované haldě
 */
bool heap_iterator_to_left(struct THeapIterator *aIter);

/** \brief Posunutí iterátoru na pravého následníka
 *  \details Funkce ověří platnost iterátoru, a pokud je platný, posune ho na pravého následníka (tj. na uzel v další vrstvě představující pravého následníka uzlu, na který směřoval iterátor \p aIter před zavoláním této funkce).
 *  \param[in,out] aIter Ukazatel na existující iterátor
 *  \return \c true pokud je iterátor platný a ukazuje i po posunutí na platné místo v asociované haldě
 */
bool heap_iterator_to_right(struct THeapIterator *aIter);

/** \brief Posunutí iterátoru na rodičovský uzel
 *  \details Funkce ověří platnost iterátoru, a pokud je platný, posune ho na uzel rodiče (tj. na uzel v předchozí vrstvě představující předchůdce uzlu, na který směřoval iterátor \p aIter před zavoláním této funkce).
 *  \param[in,out] aIter Ukazatel na existující iterátor
 *  \return \c true pokud je iterátor platný a ukazuje i po posunutí na platné místo v asociované haldě
 */
bool heap_iterator_to_parent(struct THeapIterator *aIter);

/** \brief Přečtení hodnoty uzlu haldy pomocí iterátoru
 *  \details Vrací hodnotu uzlu haldy z pozice (indexu) určeného iterátorem.
 *  \param[in] aIter Ukazatel na existující iterátor
 *  \return Hodnota uzlu haldy z pozice, na kterou ukazuje iterátor \p aIter, nebo nulový element (pokud je iterátor neplatný).
 */
[[nodiscard]] static inline THeapElement heap_iterator_value(const struct THeapIterator *aIter)
	{
	return vector_iterator_value(&aIter->iVectorIter);
	}

/** \brief Zapsání hodnoty uzlu haldy pomocí iterátoru
 *  \details Zápis nové hodnoty do uzlu haldy na pozici určenou iterátorem.
 *  \param[in] aIter Ukazatel na existující iterátor
 *  \param[in] aValue Nová hodnota uzlu haldy
 *  \return \c true pokud je iterátor platný a zápis mohl proběhnout na platné místo v asociované haldě
 */
static inline bool heap_iterator_set_value(const struct THeapIterator *aIter, THeapElement aValue)
	{
	return vector_iterator_set_value(&aIter->iVectorIter, aValue);
	}

/** \brief Přesun hodnoty uzlu haldy určené dvěma iterátory
 *  \details Přesun hodnoty uzlů haldy z/do pozice určené iterátory.
 *  \param[in] aToIter Iterátor na cílový uzel v haldě
 *  \param[in] aFromIter Iterátor na zdrojový uzel v haldě
 *  \attention Funkce ověřuje platnost obou iterátorů \b pouze při překladu v režimu `Debug`, kdy pomocí `assert` hlásí běhovou chybu!
 */
static inline void heap_move_with_iterators(const struct THeapIterator *aToIter, const struct THeapIterator *aFromIter)
	{
	vector_move_with_iterators(&aToIter->iVectorIter, &aFromIter->iVectorIter);
	}

/** \brief Porovnání hodnot uzlů haldy určených dvěma iterátory
 *  \details Trojcestně porovná hodnoty v uzlech hald určených dvěma iterátory.
 *  \param[in] aLeftIter Iterátor určující uzel haldy stojící při porovnání (LHS) vlevo
 *  \param[in] aRightIter Iterátor určující uzel haldy stojící při porovnání (RHS) vpravo
 *  \retval -1 Pokud (LHS < RHS)
 *  \retval  0 Pokud (LHS = RHS)
 *  \retval +1 Pokud (LHS > RHS)
 *  \attention Funkce ověřuje platnost obou iterátorů \b pouze při překladu v režimu `Debug`, kdy pomocí `assert` hlásí běhovou chybu!
 */
[[nodiscard]] static inline int heap_compare_with_iterators(const struct THeapIterator *aLeftIter, const struct THeapIterator *aRightIter)
	{
	return vector_compare_with_iterators(&aLeftIter->iVectorIter, &aRightIter->iVectorIter);
	}

/** \brief Porovnání mezi hodnotou v uzlu haldy určenou iterátorem a hodnotou typu HeapElement
 *  \details Trojcestně porovná hodnotu v uzlu haldy na pozici iterátorem se zadanou hodnotou \p aValue.
 *  \param[in] aIter Iterátor určující uzel haldy stojící při porovnání (LHS) vlevo
 *  \param[in] aValue Hodnota stojící při porovnání na pravé straně (RHS)
 *  \retval -1 Pokud (LHS < RHS)
 *  \retval  0 Pokud (LHS = RHS)
 *  \retval +1 Pokud (LHS > RHS)
 *  \attention Funkce ověřuje platnost iterátoru \b pouze při překladu v režimu `Debug`, kdy pomocí `assert` hlásí běhovou chybu!
 */
[[nodiscard]] static inline int heap_compare_with_iterator_value(const struct THeapIterator *aIter, THeapElement aValue)
	{
	return vector_compare_with_iterator_value(&aIter->iVectorIter, aValue);
	}

/** \brief Výměna hodnot uzlů haldy určených dvěma iterátory
 *  \details Vzájemně zamění hodnoty uzlů haldy určených dvěma iterátory.
 *  \param[in] aLeftIter Iterátor určující první uzel v haldě
 *  \param[in] aRightIter Iterátor určující druhý uzel v haldě
 *  \attention Funkce ověřuje platnost obou iterátorů \b pouze při překladu v režimu `Debug`, kdy pomocí `assert` hlásí běhovou chybu!
 */
static inline void heap_swap_with_iterators(const struct THeapIterator *aLeftIter, const struct THeapIterator *aRightIter)
	{
	vector_swap_with_iterators(&aLeftIter->iVectorIter, &aRightIter->iVectorIter);
	}

/** \brief Výměna mezi hodnotou v uzlu haldy určenou iterátorem a proměnnou typu HeapElement
 *  \details Vzájemně zamění hodnotu v uzlu haldy určenou iterátorem se zadanou hodnotou v proměnné typu HeapElement.
 *  \param[in] aIter Iterátor určující uzel haldy pro záměnu
 *  \param[in] aValue Ukazatel na hodnotu proměnné typu HeapElement určenou pro záměnu
 *  \attention Funkce ověřuje platnost iterátoru a ukazatele \b pouze při překladu v režimu `Debug`, kdy pomocí `assert` hlásí běhovou chybu!
 */
static inline void heap_swap_with_iterator_value(const struct THeapIterator *aIter, THeapElement *aValue)
	{
	vector_swap_with_iterator_value(&aIter->iVectorIter, (TVectorElement *) aValue);
	}

/** \brief Test shodnosti dvou iterátorů
 *  \details Vzájemně porovná hodnotu dvou iterátorů.
 *  \param[in] aLeftIter První iterátor (LHS)
 *  \param[in] aRightIter Druhý iterátor (RHS)
 *  \return \c true pokud iterátory odkazují na uzel na stejné pozici a ve stejném haldě případně jsou oba neplatné
 */
[[nodiscard]] static inline bool heap_iterator_is_equal(const struct THeapIterator *aLeftIter, const struct THeapIterator *aRightIter)
	{
	return vector_iterator_is_equal(&aLeftIter->iVectorIter, &aRightIter->iVectorIter);
	}

/** \brief Porovnání dvou iterátorů
 *  \details Provede trojcestné porovnání hodnot dvou iterátorů, předaných pomocí ukazatelů. Oba iterátory musí vzniknout ze stejné haldy.
 *  \param[in] aLeftIter Levý iterátor (tzv. LHS - Left Hand Side)
 *  \param[in] aRightIter Pravý iterátor (tzv. RHS - Right Hand Side)
 *  \retval -1 Pokud je pozice v \p aLeftIter \b < než pozice v \p aRightIter, nebo je \p aLeftIter \b neplatný a \p aRightIter platný
 *  \retval  0 Pokud je pozice v \p aLeftIter \b = pozici v \p aRightIter, nebo jsou oba iterátory \b neplatné
 *  \retval +1 Pokud je pozice v \p aLeftIter \b > než pozice v \p aRightIter, nebo je \p aLeftIter platný a \p aRightIter \b neplatný
 *  \attention Funkce ověřuje, že oba iterátory vznikly ze stejné haldy \b pouze při překladu v režimu `Debug`, kdy pomocí `assert` hlásí běhovou chybu!
 */
[[nodiscard]] static inline int heap_iterator_comparator(const struct THeapIterator *aLeftIter, const struct THeapIterator *aRightIter)
	{
	return vector_iterator_comparator(&aLeftIter->iVectorIter, &aRightIter->iVectorIter);
	}

/** \} THeap */

/** \defgroup HeapAlgorithms 2.2.2.1. Univerzální funkce pro práci s haldou
 *  \brief Definice funkcí používající haldu
 *  \ingroup THeap
 *  \{
 */

/** \brief Tisk elementů haldy
 *  \details Vytiskne formátovaně elementy ve tvaru haldy i ve tvaru vektoru.
 *  \param[in] aHeap Ukazatel na existující haldu
 *  \param[in,out] aOutputFile Ukazatel na soubor otevřený v módu pro zápis
 */
void heap_dump(const struct THeap *aHeap, FILE *aOutputFile);

/** \} HeapAlgorithms */

#endif /* THEAP_H */
