/** \file wildcard_match_ut.cpp
 *  \brief Definice metod pro automatizované testování správnosti implementace funkce wildcard_match() v prostředích: M$ Visual Studio nebo Apple XCode
 *  \author Petyovský
 *  \version 2026
 *  $Id: wildcard_match_ut.cpp 3307 2026-03-30 14:12:07Z petyovsky $
 */

/** \defgroup UnitTests 3. Automatizované testování správnosti implementace
 *  \brief Definice testovacích metod pro automatizované testování správnosti implementace funkce wildcard_match() v prostředí M$ Visual Studio nebo Apple XCode
 *  \details Automatizované testování správnosti implementace funkce wildcard_match() je zajištěno tzv. unittesty.
 Mechanismus Unit testů funguje tak, že využívá zdrojové a přeložené soubory testovaného projektu a ověřuje správnost implementace jednotlivých (jednotek) v našem případě funkcí (testované soubory tedy nekopírujeme, ale jen mezi oběma projekty sdílíme). Pro testování je připraven kód tak, aby získal voláním výsledek určité jednotky (v nejjednodušším případě zavoláme testovanou funkci). Pomocí funkcí ze skupiny UT_ASSERT porovnáme shodu či neshodu výsledků s požadavkem. V případě, že se výsledek funkce neshoduje s předpokládaným výsledkem, běh tohoto testu se přeruší a výsledek se zobrazí v prostředí a pokračuje se na další test v pořadí.

 Seznam testů naleznete v záložce: \ref test "Ostatní stránky > Seznam testů".
 *  \{
 */

#include "ut_adapter.h"

#ifdef __cplusplus
	extern "C"
		{
#endif

#include "wildcard_match.h"

#ifdef __cplusplus
		}
#endif

/** \} UnitTests */

UT_NAMESPACE(wildcard_match_ut)

	/// \test \b testEmptyString:	`("", "") -> true`
	/// \test \b testSameChars:		`("a", "a") -> true`
	/// \test \b testDifferChars:	`("a", "b") -> false`, (because \c 'a' is not \c 'b').
	/// \test \b testSameStrings:	`("ab", "ab") -> true`
	/// \test \b testDifferString:	`("ab", "ca") -> false`, (because strings differ).

	UT_CLASS(StringMatchTest)

		UT_METHOD(testEmptyStrings)
			{
			UT_ASSERT_IS_TRUE(wildcard_match("", ""));
			}

		UT_METHOD(testSameChars)
			{
			UT_ASSERT_IS_TRUE(wildcard_match("a", "a"));
			}

		UT_METHOD(testDifferChars)
			{
			UT_ASSERT_IS_FALSE(wildcard_match("a", "b"));			// It must be false, because 'a' is not 'b'
			}

		UT_METHOD(testSameStrings)
			{
			UT_ASSERT_IS_TRUE(wildcard_match("ab", "ab"));
			}

		UT_METHOD(testDifferString)
			{
			UT_ASSERT_IS_FALSE(wildcard_match("ab", "ca"));			// It must be false, because strings differ
			}

	UT_ENDCLASS(StringMatchTest)


	/// \test \b testQuestionWildcardNotMatchEmpty:	`("?", "") -> false`
	/// \test \b testQuestionWildcardMatchAny:		`("?", "a") -> true`

	UT_CLASS(QuestionWildcardMatchTest)

		UT_METHOD(testQuestionWildcardNotMatchEmpty)
			{
			UT_ASSERT_IS_FALSE(wildcard_match("?", ""));
			}

		UT_METHOD(testQuestionWildcardMatchAny)
			{
			UT_ASSERT_IS_TRUE(wildcard_match("?", "a"));
			}

	UT_ENDCLASS(QuestionWildcardMatchTest)


	/// \test \b testStarMatchEmpty:			`("*", "") -> true`
	/// \test \b testStarMatchOneChar:			`("*", "a") -> true`
	/// \test \b testStarMatchOneCharInMiddle:	`("a*cd", "abcd") -> true`
	/// \test \b testStarMatchAllString:		`("*", "aaaa") -> true`
	/// \test \b testNotMatchDifferString:		`("a*c", "abb") -> false`, (because \c 'c' is not in \p aStr).
	/// \test \b testNotMatchLongerString:		`("*pqrs", "pqrst") -> false`, (because \c 't' is not in \p aWildCardStr).
	/// \test \b testMatchLongerString:			`("abc*bcd", "abcdhghgbcd") -> true`
	/// \test \b testStarMatchStringInMiddle:	`("d*d", "dcabad") -> true`
	/// \test \b testTwoStarMatchEmpty:			`("**", "") -> true`
	/// \test \b testTwoStarMatchChar:			`("**", "a") -> true`
	/// \test \b testTwoStarMatchAny:			`("**", "aaaa") -> true`
	/// \test \b testStarCharStarMatchChar:		`("*a*", "a") -> true`
	/// \test \b testStarCharStarCharMatch:		`("*c*d", "abcd") -> true`
	/// \test \b testThreeStarMatchEmpty:		`("***", "") -> true`

	UT_CLASS(StarWildcardMatchTest)

		UT_METHOD(testStarMatchEmpty)
			{
			UT_ASSERT_IS_TRUE(wildcard_match("*", ""));
			}

		UT_METHOD(testStarMatchOneChar)
			{
			UT_ASSERT_IS_TRUE(wildcard_match("*", "a"));
			}

		UT_METHOD(testStarMatchOneCharInMiddle)
			{
			UT_ASSERT_IS_TRUE(wildcard_match("a*cd", "abcd"));
			}

		UT_METHOD(testStarMatchAllString)
			{
			UT_ASSERT_IS_TRUE(wildcard_match("*", "aaaa"));
			}

		UT_METHOD(testNotMatchDifferString)
			{
			UT_ASSERT_IS_FALSE(wildcard_match("a*c", "abb"));		// It must be false, because 'c' is not in aStr
			}

		UT_METHOD(testNotMatchLongerString)
			{
			UT_ASSERT_IS_FALSE(wildcard_match("*pqrs", "pqrst"));	// It must be false, because 't' is not in aWildCardStr
			}

		UT_METHOD(testMatchLongerString)
			{
			UT_ASSERT_IS_TRUE(wildcard_match("abc*bcd", "abcdhghgbcd"));
			}

		UT_METHOD(testStarMatchStringInMiddle)
			{
			UT_ASSERT_IS_TRUE(wildcard_match("d*d", "dcabad"));
			}

		UT_METHOD(testTwoStarMatchEmpty)
			{
			UT_ASSERT_IS_TRUE(wildcard_match("**", ""));
			}

		UT_METHOD(testTwoStarMatchChar)
			{
			UT_ASSERT_IS_TRUE(wildcard_match("**", "a"));
			}

		UT_METHOD(testTwoStarMatchAny)
			{
			UT_ASSERT_IS_TRUE(wildcard_match("**", "aaaa"));
			}

		UT_METHOD(testStarCharStarMatchChar)
			{
			UT_ASSERT_IS_TRUE(wildcard_match("*a*", "a"));
			}

		UT_METHOD(testStarCharStarCharMatch)
			{
			UT_ASSERT_IS_TRUE(wildcard_match("*c*d", "abcd"));
			}

		UT_METHOD(testThreeStarMatchEmpty)
			{
			UT_ASSERT_IS_TRUE(wildcard_match("***", ""));
			}

	UT_ENDCLASS(StarWildcardMatchTest)


	/// \test \b testStarQuestionStarNotMatchEmpty:	`("*?*", "") -> false`, (because \p aStr must have at least one character).
	/// \test \b testStarQuestionStarMatchChar:		`("*?*", "a") -> true`
	/// \test \b testQuestionAndStarAtEndMatch:		`("ab?de*", "abcdefghij") -> true`
	/// \test \b testStarCharQuestionNotMatch:		`("abc*c?d", "abcd") -> false`, (because \p aStr must have 2 instances of \c 'c').
	/// \test \b testStarQuestionCharStarMatch:		`("*?c*d", "abcd") -> true`

	UT_CLASS(AdvancedWildcardMatchTest)

		UT_METHOD(testStarQuestionStarNotMatchEmpty)
			{
			UT_ASSERT_IS_FALSE(wildcard_match("*?*", ""));			// It must be false, because aStr must have at least one character
			}

		UT_METHOD(testStarQuestionStarMatchChar)
			{
			UT_ASSERT_IS_TRUE(wildcard_match("*?*", "a"));
			}

		UT_METHOD(testQuestionAndStarAtEndMatch)
			{
			UT_ASSERT_IS_TRUE(wildcard_match("ab?de*", "abcdefghij"));
			}

		UT_METHOD(testStarCharQuestionNotMatch)
			{
			UT_ASSERT_IS_FALSE(wildcard_match("abc*c?d", "abcd"));	// It must be false, because aStr must have 2 instances of 'c'
			}

		UT_METHOD(testStarQuestionCharStarMatch)
			{
			UT_ASSERT_IS_TRUE(wildcard_match("*?c*d", "abcd"));
			}

	UT_ENDCLASS(AdvancedWildcardMatchTest)

UT_ENDNAMESPACE(wildcard_match_ut)
