/** \file main.c
 *  \brief Hlavní program (testbench) testující různé implementace výpočtu hodnot Fibonacciho posloupnosti
 *  \author Petyovský
 *  \version 2026
 *  $Id: main.c 3307 2026-03-30 14:12:07Z petyovsky $
 */

#include <stdio.h>
#include <time.h>
#include "main_support.h"
#include "fibonacci.h"
#include "check.h"

/** \defgroup MainProgram 2. Hlavní program
 *  \brief Definice funkcí hlavního programu
 *  \{
 */

/** \brief Definice typu TTestSetItem
 *  \details Typ představuje jeden pár hodnot pro: vstup a výstup (návratová hodnota) pro hromadné testování funkcí fibonacciX
 */
struct TTestSetItem
	{
	unsigned iInput;		///< Požadovaná vstupní hodnota funkce
	unsigned iOutput;		///< Očekávaná návratová hodnota funkce
	};

/** \brief Testování funkcí fibonacciX()
 *  \details Testovací funkce pro API fibonacciX(), funkce změří a vytiskne celkový výpočetní čas a velikost paměti, kterou funkce potřebovala k dokončení výpočtu.
 *  \param[in] aFibFuncNameStr String reprezentující název funkce pro výpočet Fibonacciho čísla
 *  \param[in] aFibFunc Ukazatel na funkci reprezentující výpočet Fibonacciho čísla
 *  \param[in] aTestSetSize Celkový počet kontrolních výpočtů Fibonacciho čísla pomocí zadané funkce \p aFibFunc
 *  \param[in] aTestSet Pole vstupních hodnot a správných návratových hodnot pro výpočet Fibonacciho čísla
 *  \return \c true pokud test pro daném testovací pole dopadl úspěšně
 */
bool fibonacci_testing(const char *aFibFuncNameStr, unsigned (*aFibFunc)(unsigned), size_t aTestSetSize, const struct TTestSetItem aTestSet[])
	{
	printf("\n------ %s ------\n", aFibFuncNameStr);
	puts("Testset is:");
	for(size_t i = 0; i < aTestSetSize; ++i)
		printf("\tfibonacci(%u) = %u\n", aTestSet[i].iInput, aTestSet[i].iOutput);
	puts("\nTesting started...");

	struct timespec start_timespec = { 0, }, end_timespec = { 0, };
	UNLESS(timespec_get(&start_timespec, TIME_UTC));

	for(size_t i = 0; i < aTestSetSize; ++i)
		{
		unsigned result = (*aFibFunc)(aTestSet[i].iInput);
		if(result != aTestSet[i].iOutput)
			{
			printf("\tfibonacci(%u) = %u, is wrong (must be %u)!! FAIL :-(\n", aTestSet[i].iInput, result, aTestSet[i].iOutput);
			return false;
			}
		}

	UNLESS(timespec_get(&end_timespec, TIME_UTC));

	puts("Testing finished...");
	printf("All results was valid. OK :-)\n");
	printf("Calculation time: %lf seconds.\n", difftimespec(end_timespec, start_timespec));
	return true;
	}

int main(int argc, char *argv[])
	{
	const struct TTestSetItem testset[] =
		{
			{ 0,             0u},	//fibonacci(0)  = 0
			{ 1,             1u},	//fibonacci(1)  = 1
			{ 3,             2u},	//fibonacci(3)  = 2
			{ 5,             5u},	//fibonacci(5)  = 5
			{42,   267'914'296u},	//fibonacci(42) =   267'914'296
			{45, 1'134'903'170u},	//fibonacci(45) = 1'134'903'170
			{46, 1'836'311'903u},	//fibonacci(46) = 1'836'311'903
			{47, 2'971'215'073u},	//fibonacci(47) = 2'971'215'073
		};

	//-----------------------------------------------------------------------
	// Test implementace stromovou rekurzí
	// Na pomalých PC nahraď výraz: `ITEMS_OF(testset)` za: `ITEMS_OF(testset) - 1`
	for(size_t i = 0; i < ITEMS_OF(testset)/* - 1 */; ++i)
		fibonacci_testing("Test of fibonacci1 - tree recursion", fibonacci1, 1, &testset[i]);

	//-----------------------------------------------------------------------
	// Test implementace iterací
	fibonacci_testing("Test of fibonacci2 - iteration", fibonacci2, ITEMS_OF(testset), testset);

	//-----------------------------------------------------------------------
	// Test implementace stromovou rekurzí s memoizací
	UNLESS(memoizer_init(&m_fibonacci3, 2, fibonacci3), goto cleanup_main_memoizer);

	for(size_t i = 0; i < ITEMS_OF(testset); ++i)
		{
		fibonacci_testing("Test of fibonacci3 - tree recursion with memoization", fibonacci3_wrapper, 1, &testset[i]);
		UNLESS(memoizer_print_stats(&m_fibonacci3, stdout), goto cleanup_main_memoizer);
		UNLESS(memoizer_clear(&m_fibonacci3), goto cleanup_main_memoizer);
		}

	fibonacci_testing("Test of fibonacci3 - tree recursion with memoization (all together)", fibonacci3_wrapper, ITEMS_OF(testset), testset);
	UNLESS(memoizer_print_stats(&m_fibonacci3, stdout), goto cleanup_main_memoizer);
	memoizer_destroy(&m_fibonacci3);

	//-----------------------------------------------------------------------
	// Test implementace s koncovou rekurzí
	fibonacci_testing("Test of fibonacci4 - tail call recursion", fibonacci4, ITEMS_OF(testset), testset);
	return 0;

cleanup_main_memoizer:
	memoizer_destroy(&m_fibonacci3);
	return 1;
	}

// Pro další studium pokračujte sem: https://en.wikipedia.org/wiki/Jacques_Philippe_Marie_Binet
// A potom sem: https://en.wikipedia.org/wiki/Golden_ratio

/** \example cv08_reports_fibonacci_i7-3770_x64_vs2019_debug.lst
 * Příklad výstupu programu na platformě Intel(R) Core(TM) i7-3770 CPU @ 3.40GHz, VS 2019 v Debug režimu x64.
 */
/** \example cv08_reports_fibonacci_i7-3770_x64_vs2019_release.lst
 * Příklad výstupu programu na platformě Intel(R) Core(TM) i7-3770 CPU @ 3.40GHz, VS 2019 v Release režimu x64.
 */

/** \example cv08_reports_fibonacci_i3-8100_x64_vs2022_debug.lst
 * Příklad výstupu programu na platformě Intel(R) Core(TM) i3-8100 CPU @ 3.60GHz, VS 2022 v Debug režimu x64.
 */
/** \example cv08_reports_fibonacci_i3-8100_x64_vs2022_release.lst
 * Příklad výstupu programu na platformě Intel(R) Core(TM) i3-8100 CPU @ 3.60GHz, VS 2022 v Release režimu x64.
 */

/** \example cv08_reports_fibonacci_i7-13700_x64_vs2022_debug.lst
 * Příklad výstupu programu na platformě Intel(R) Core(TM) i7-13700 CPU @ 2.10GHz, VS 2022 v Debug režimu x64.
 */
/** \example cv08_reports_fibonacci_i7-13700_x64_vs2022_release.lst
 * Příklad výstupu programu na platformě Intel(R) Core(TM) i7-13700 CPU @ 2.10GHz, VS 2022 v Release režimu x64.
 */

 /** \example cv08_reports_fibonacci_i7-13700_x64_vs2026_debug.lst
  * Příklad výstupu programu na platformě Intel(R) Core(TM) i7-13700 CPU @ 2.10GHz, VS 2026 v Debug režimu x64.
  */
  /** \example cv08_reports_fibonacci_i7-13700_x64_vs2026_release.lst
   * Příklad výstupu programu na platformě Intel(R) Core(TM) i7-13700 CPU @ 2.10GHz, VS 2026 v Release režimu x64.
   */

/** \example cv08_reports_fibonacci_m1-max_xcode14_debug.lst
 * Příklad výstupu programu na platformě Apple M1 Max CPU @ 3.228Ghz, XCode 14 v Debug režimu ARM64.
 */
/** \example cv08_reports_fibonacci_m1-max_xcode14_release.lst
 * Příklad výstupu programu na platformě Apple M1 Max CPU @ 3.228Ghz, XCode 14 v Release režimu ARM64.
 */

/** \example cv08_reports_fibonacci_m3-pro_xcode15_debug.lst
 * Příklad výstupu programu na platformě Apple M3 Pro CPU @ 4.05Ghz, XCode 15 v Debug režimu ARM64.
 */
/** \example cv08_reports_fibonacci_m3-pro_xcode15_release.lst
 * Příklad výstupu programu na platformě Apple M3 Pro CPU @ 4.05Ghz, XCode 15 v Release režimu ARM64.
 */

/** \example cv08_reports_fibonacci_m3-pro_xcode17_debug.lst
 * Příklad výstupu programu na platformě Apple M3 Pro CPU @ 4.05Ghz, XCode 26 (17) v Debug režimu ARM64.
 */
/** \example cv08_reports_fibonacci_m3-pro_xcode17_release.lst
 * Příklad výstupu programu na platformě Apple M3 Pro CPU @ 4.05Ghz, XCode 26 (17)  v Release režimu ARM64.
 */
/** \} MainProgram */
