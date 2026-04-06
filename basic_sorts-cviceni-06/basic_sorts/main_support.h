#ifndef MAIN_SUPPORT_H
#define MAIN_SUPPORT_H
/** \file main_support.h
 *  \brief Soubor pro multiplatformní podporu programu v různých OS
 *  \author Petyovský
 *  \version 2026
 *  $Id: main_support.h 3246 2026-03-10 13:54:55Z petyovsky $
 */

#include <stdio.h>
#include <time.h>
#include <stdbool.h>

#ifndef ITEMS_OF
 #define ITEMS_OF(aArray) (sizeof(aArray) / sizeof((aArray)[0]))	///< Makro vracející počet prvků pole předaného jako parametr \p aArray
#endif

#ifndef UNLESS
#define UNLESS(aPredicate, ...)		\
 do									\
	{								\
	if(!(aPredicate))				\
		{							\
		fprintf(stderr, "\nExpression '%s' failed in the function: '%s()' at line '%u' of the source file:\n\t'%s'\n%s\n", #aPredicate, __func__, __LINE__, __FILE__, #__VA_ARGS__ ); \
		__VA_ARGS__;				\
		}							\
	}								\
 while(0)	///< Makro kontrolující hodnotu výrazu \p aPredicate, při neúspěchu vytiskne chybového hlášení a následně provede zadaný příkaz předaný jako druhý parametr (pokud je uveden).
#endif /* UNLESS */

#define TO_CSTR(aPar) #aPar			///< Makro pro převod identifikátoru \p aPar na řetězec: `TO_CSTR(blabla)` na `"blabla"`

/** \brief Konverzní funkce z hodnoty typu \c bool na C-řetězec
 *  \details Konverzní funkce vracející konstatní řetězec \c "true" nebo \c "false" dle hodnoty vstupního parametru \p aValue typu \c bool.
 *  \param[in] aValue Boolean hodnota
 *  \return Vrací ukazatel na konstatní řetězec \c "true" nebo \c "false"
 */
static inline const char* bool_to_cstring(const bool aValue)
	{
	return (aValue) ? TO_CSTR(true) : TO_CSTR(false);
	}

/** \brief Výpočet doby trvání časového intervalu
 *  \details Výpočte délku trvání časového intervalu předaného jako parametry \p aEnd a \p aBegin.
 *  \param[in] aEnd Konec časového intervalu
 *  \param[in] aBegin Začátek časového intervalu
 *  \return Doba trvání časového intervalu (desetinné číslo v sekundách typu \c double)
 */
static inline double difftimespec(struct timespec aEnd, struct timespec aBegin)
	{
	return difftime(aEnd.tv_sec, aBegin.tv_sec) + (((long long) aEnd.tv_nsec - aBegin.tv_nsec) / 1000000000.0);
	}

#ifdef __MACH__ // Detect macOS
	#include <Availability.h>
	#if __MAC_OS_X_VERSION_MIN_REQUIRED < 101500 // macOS < 10.15 (pre Catalina) does not have timespec_get, use clock_get_time
		#include <mach/clock.h>
		#include <mach/mach.h>

		#define TIME_UTC 1
		static inline int timespec_get(struct timespec *ts, int unused)
			{
			clock_serv_t cclock;
			mach_timespec_t mts;
			host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
			clock_get_time(cclock, &mts);
			mach_port_deallocate(mach_task_self(), cclock);
			ts->tv_sec = mts.tv_sec;
			ts->tv_nsec = mts.tv_nsec;
			return unused;
			}
	#endif // __MAC_OS_X_VERSION_MIN_REQUIRED
#endif // __MACH__

#endif /* MAIN_SUPPORT_H */
