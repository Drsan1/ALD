/** \file fibonacci.c
 *  \brief Definice různých variant implementace výpočtu hodnot Fibonacciho posloupnosti
 *  \author Petyovský
 *  \version 2026
 *  $Id: fibonacci.c 3307 2026-03-30 14:12:07Z petyovsky $
 */

#include "fibonacci.h"

//-----------------------------------------------------------------------
// Implementace stromovou rekurzí
unsigned fibonacci1(unsigned n)
	{
	if (n < 2)
		return n;

	return fibonacci1(n - 1) + fibonacci1(n - 2);
	}

//-----------------------------------------------------------------------
// Implementace iterací
unsigned fibonacci2(unsigned n)
	{
	if (n < 2)
		return n;

	unsigned fib_n_1 = 1;	// fibonacci(n-1)
	unsigned fib_n_2 = 0;	// fibonacci(n-2)
	for (unsigned i = 0; i < n - 1; i++) {
		unsigned tmp = fib_n_1 + fib_n_2;
		fib_n_2 = fib_n_1;
		fib_n_1 = tmp;
	}
	return fib_n_1;
	}
//-----------------------------------------------------------------------
// Implementace stromovou rekurzí s memoizací
struct TMemoizer m_fibonacci3 = { 0, };	// Instance memoizeru pro funkci fibonacci3()

unsigned fibonacci3(unsigned n)
	{
	if (n < 2)
		return n;
	
	//return fibonacci3_wrapper(n - 1) + fibonacci3_wrapper(n - 2);
	return fibonacci3_wrapper(n - 2) + fibonacci3_wrapper(n - 1);
	}

unsigned fibonacci3_wrapper(unsigned n)
	{
	return memoizer_eval(&m_fibonacci3, n);
	}

//-----------------------------------------------------------------------
// Implementace s koncovou rekurzí
/** \brief Vnitřní, lokální pracovní funkce realizující koncovou rekurzi
 */
[[nodiscard]] static unsigned fibonacci4_worker(unsigned n, unsigned fib_n_1, unsigned fib_n_2)
	{
	if (n < 1)
		return fib_n_2; 
	return fibonacci4_worker(n - 1, fib_n_1 + fib_n_2, fib_n_1);
	}	   
		   
unsigned fibonacci4(unsigned n)
	{	   
	return fibonacci4_worker(n, 1, 0);
	}




			  










