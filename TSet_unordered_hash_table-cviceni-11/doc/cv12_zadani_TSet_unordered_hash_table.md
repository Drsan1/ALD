# Zadání cvičení 12

Cvičení je věnováno návrhu a implementaci ADT množina (viz přednáška č. 5) ukládající prvky do hašovací tabulky využívající otevřené adresování.
Zadání obsahuje zdrojové soubory projektu představující **nedokončenou implementaci ADT Množina s prvky uloženými v hašovací tabulce** - `TSet_unordered_hash_table`.
Otevřené adresování realizujte pro **lineární** i **kvadratické** dohledávání (zkoumání) následující pozice (viz přednáška č. 12) - (*Linear probing*, *Quadratic probing*).

---

## Zadání úlohy
V dodaném projektu `TSet_unordered_hash_table` dokončete ve zdrojovém souboru `TSetHashTableOpenAddr.c` implementaci funkcí:  \code{.c}

	// Vyhledání elementu o zadané hodnotě v hašovací tabulce.
	int set_hash_table_search(const struct TSetHashTable *aTable, TSetElement aValue, size_t *aPosPtr)
	
	// Vložení elementu do hašovací tabulky.
	bool set_hash_table_insert(struct TSetHashTable *aTable, TSetElement aValue)
	
	// Odstranění elementu z hašovací tabulky.
	bool set_hash_table_erase(struct TSetHashTable *aTable, TSetElement aValue)
\endcode   

Implementaci funkce `set_hash_table_search()` realizujte tak, aby podporovala pro hašovací tabulku operace:

  1. *lineárního dohledávání* následující platné pozice (při definici makra: `HASH_TABLE_SEARCH_VARIANT == LINEAR_PROBING`),
  2. *kvadratického dohledávání* následující platné pozice (při definici makra: `HASH_TABLE_SEARCH_VARIANT == QUADRATIC_PROBING`).

Jednotlivé operace vkládání a odebírání prvků realizujte tak, aby ke své funkci využívaly k vyhledání prvků funkci `set_hash_table_search()`.

Algoritmy vyzkoušejte na testovacích posloupnostech využívaných ve funkci `main()`, které jsou součástí projektu.

Srovnejte výsledné počty komparací prvků množiny z vašeho programu s hodnotami v referenčních souborech s výsledky: [`doc/cv12_reports_*.lst`](examples.html).

### Soubory projektu

Projekt `TSet_unordered_hash_table` není dokončen. Ve zdrojovém souboru `TSetHashTableOpenAddr.c` chybí těla funkcí realizující operace pro práci s hašovací tabulkou.
Na začátku tohoto souboru je také pomocí `#define` realizováno přepínání jednotlivých variant dohledávání následující platné pozice tabulky.

Soubor `TSetElementHashable.h` - obsahuje funkce pro porovnání prvků množiny a mechanismus určení počtu těchto porovnání. Nově je zde doplněna funkce výpočtu hodnoty hašovací funkce pro zadaný element: `set_element_hash_value()`.
Pro evidenci počtu porovnání opětovně slouží globální proměnná *gSetElementComparisons*.

Soubory `check.c` a `check.h` - soubory pro kontrolu manipulace s pamětí a soubory.

Soubor `main.c` - obsahuje základní a manuálně naimplementovanou testovací sekvenci ve formě programu uvnitř funkce `testset_hardcoded()`.
Testovací sekvence začíná voláním funkce `set_init()` a končí voláním funkce `set_destroy()`.
Dále je zde realizováno volání funkcí pro vkládání, výběr, tisk a zjišťování přítomnosti 
prvku v množině. Tisk je prováděn pomocí iterátorů `struct TSetIterator`. Ve zdrojovém souboru je také implementována podpora pro testování pomocí test plánů.

Soubory `TSet.c` a `TSet.h` jsou kompletně naimplementovány a obsahují také všechna volání funkcí ze souboru: `TSetHashTableOpenAddr.h`.

 - Soubor `TSet.h` je hlavičkový soubor, který obsahuje definici
    struktury množiny `struct TSet` a struktury iterátoru pro množinu `struct TSetIterator`.
    Rozhraní funkcí je podobné jako v minulých projektech a názvy funkcí dostatečně popisují 
    jejich činnost.

 - Vlastní množinu reprezentuje struktura `struct TSet` využívající hašovací tabulku
    viz složka `iHashTable` a složka `iSize` uchovávající aktuálně počet prvků uložených v množině.

 - Soubor `TSet.c` obsahuje tyto funkce:
    - `set_init()` - inicializace struktury množiny,
    - `set_size()` - zjištění počtu elementů množiny (neboli velikost množiny),
    - `set_is_empty()` - test, zda je množina prázdná,
    - `set_insert()` - vložení prvku do množiny realizované hašovací tabulkou. V případě neúspěchu vrací `false`, jinak vrací `true`,
    - `set_erase()` - odebrání prvku z množiny realizované hašovací tabulkou. V případě neúspěchu vrací `false`, jinak vrací `true`,
    - `set_is_element_of()` - zjištění přítomnosti dané hodnoty v množině,
    - `set_find()` - vytvoření iterátoru ukazujícího na danou hodnotu prvku v množině,
    - `set_destroy()` - korektní ukončení života proměnné typu množina.
   
   A také základní operace pro práci s iterátory této množiny:
    - `set_iterator_begin()` - vytvoření iterátoru směřujícího na první element množiny,
    - `set_iterator_is_valid()` - predikátová funkce vracející `true`, pokud iterátor směřuje na platný element množiny,
    - `set_iterator_to_next()` - posun iterátoru na další element množiny,
    - `set_iterator_value()` - získání hodnoty elementu množiny, na který směřuje iterátor.

Soubor `TSetHashTableOpenAddr.h` je kompletní a soubor `TSetHashTableOpenAddr.c` je realizován pouze částečně a obsahuje:

 - Hotovou implementaci těchto operací nad hašovací tabulkou využívající otevřené adresování:
     - `set_hash_table_init()` - alokace a inicializace hašovací tabulky,
     - `set_hash_table_destroy()` - deinicializace a dealokace hašovací tabulky a elementů v ní obsažených,
     - `set_hash_table_begin_pos()` - výpočet první platné pozice přihrádky v hašovací tabulce (*Využíváno k realizaci iterátoru nad hašovací tabulkou.*),
     - `set_hash_table_is_valid_pos()` - zjištění platnosti pozice přihrádky v hašovací tabulce (*Využíváno k realizaci iterátoru nad hašovací tabulkou.*),
     - `set_hash_table_next_pos()` - výpočet následující pozice platné přihrádky v hašovací tabulce (*Využíváno k realizaci iterátoru nad hašovací tabulkou.*),
     - `set_hash_table_value_at_pos()` - získání hodnoty elementu z platné přihrádky v hašovací tabulce. (*Využíváno k realizaci iterátoru nad hašovací tabulkou.*)

 - **Nedokončenou implementaci** těchto operací nad hašovací tabulkou. **Pro porovnání elementů nezapomeňte v těchto operacích používat předdefinovanou funkci `set_element_comparator()` ze souboru `TSetElementHashable.h`**. V rámci realizace je proto nutné implementovat tyto funkce:
     - `set_hash_table_search()` - vyhledání elementu o zadané hodnotě v hašovací tabulce. Funkci implementujte tak, 
       aby podporovala otevřené adresování pro obě varianty: *lineárního* i *kvadratického* dohledávání následující pozice v hašovací tabulce.
       Požadovaný význam návratových hodnot naleznete v dokumentaci funkce.
     - `set_hash_table_insert()` - vložení elementu do hašovací tabulky. (Tato funkce bude využívat: `set_hash_table_search()`.),
     - `set_hash_table_erase()` - odstranění elementu z hašovací tabulky. (Tato funkce bude využívat: `set_hash_table_search()`.)

 - Vzorové výsledky testování obsahující počty porovnání jsou pro kontrolu správnosti vašeho řešení dostupné v souborech:
     - pro *lineární dohledávání* v: [`doc/cv12_reports_testplan_set*_linear_probing.lst`](examples.html),
     - pro *kvadratické dohledávání* v: [`doc/cv12_reports_testplan_set*_quadratic_probing.lst`](examples.html).

### Úkoly

 1. Prostudujte si hierarchii zdrojových souborů v dodaném projektu.
 2. Projekt obsahuje automaticky konfigurovatelné testy v souborech [`testplan_set*.h`](files.html), prostudujte si jejich výhody namísto využívání ručně napsaného textu ve funkci
    `testset_hardcoded()`. Uvědomte si, proč je automatický tester realizovaný ve zdrojových souborech `testbench.c` a `testbench.h` výhodnější a naučte se vytvářet vlastní test plán.
 3. Zdrojové soubory `TSet.h` a `TSet.c` realizující neuspořádanou množinu jsou kompletně hotové. Nastudujte si jejich obsah a smysl volání servisních funkcí deklarovaných v souboru `TSetHashTableOpenAddr.c`.
 4. Promyslete si jednotlivé implementace operací vkládání, odebrání a vyhledávání prvků v hašovací tabulce ve zdrojovém souboru `TSetHashTableOpenAddr.c`. Uvědomte si důvod pro existenci speciální hodnoty: `Marked_as_erased` získané pomocí funkce `calc_erased_bucket_marker()`, která označuje původně použitou, ale nyní již prázdnou, přihrádku (tzv. [bucket](https://en.wikipedia.org/wiki/Bucket_sort)).
 5. Doplňte chybějící části těl jednotlivých funkcí ve zdrojovém souboru `TSetHashTableOpenAddr.c`.
 6. Ve funkci `set_hash_table_search()` realizujte nejprve variantu vyhledávání elementu pomocí lineárního dohledávání následující platné pozice a otestujte její fungování.
 7. Následně ve stejné funkci realizujte variantu vyhledávání elementu pomocí kvadratického dohledávání následující platné pozice a opět otestujte její fungování.
 8. Realizované algoritmy otestujte na připravených souborech [`testplan_set*.h`](files.html) pomocí funkce `main()`.
    Srovnejte výsledné hodnoty z vašeho programu s hodnotami v souborech s výsledky, které mají název začínající [`doc/cv12_reports_*.lst`](examples.html). Zároveň si prohlédněte
    zobrazené hodnoty z vašich metod a ujasněte si průběh procesu vyhledávání pro lineární a kvadratického dohledávání.   
    Pokuste se dosáhnout stejných výsledků pro počet komparací i alokované paměti jako je uvedeno v přiložených souborech, aniž byste museli modifikovat obsah
    souboru: `main.c`.

### Rozbor úlohy

Viz přednášky č. 5 (ADT Množina) a č. 12 (Hašovací tabulka). Pro experimentování můžete také využít [online vizualizační nástroj](https://www.cs.usfca.edu/~galles/visualization/ClosedHash.html).

---
`$Id: cv12_zadani_TSet_unordered_hash_table.md 3368 2026-04-27 13:51:34Z petyovsky $`
