#ifndef UT_ADAPTER_H
#define UT_ADAPTER_H
/** \file ut_adapter.h
 *  \brief Unit Test adapter for M$ Visual Studio and Apple XCode main include file
 *  \author Petyovský
 *  \version 2025
 *  $Id: ut_adapter.h 3014 2025-03-19 16:53:40Z petyovsky $
 */

#ifdef _MSC_VER
	#include "ut_adapter_vs.h"

#elif defined(__MACH__)
	#include "ut_adapter_xc.h"

#else
	#error "Unknown compiler infrastructure"

#endif

#endif /* UT_ADAPTER_H */
