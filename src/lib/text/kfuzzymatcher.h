/*
    This file is part of the KDE libraries

    SPDX-FileCopyrightText: 2017 Forrest Smith <forrestthewoods@gmail.com>
    SPDX-FileCopyrightText: 2021 Waqar Ahmed <waqar.17a@gmail.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include <kcoreaddons_export.h>

class QString;
class QStringView;

/**
 * This namespace contains functions for fuzzy matching of strings.
 *
 * This code is ported to Qt from lib_fts:
 * https://github.com/forrestthewoods/lib_fts
 * which tries to replicate SublimeText like fuzzy matching.
 *
 * In general match() and matchSequential() will suffice for most usecases
 * but should the need arise to extend this for some particular reason, for example
 * scoring based on a different kind of separator, it should ideally be handled in a
 * separate internal method with an interface similar to the existing ones.
 *
 * All methods in here are stateless i.e., the input string will not be modified.
 *
 * @short Namespace for fuzzy matching of strings
 * @author Waqar Ahmed <waqar.17a@gmail.com>
 */
namespace KFuzzyMatcher
{

/**
 * @brief The Result struct
 */
struct KCOREADDONS_EXPORT Result
{
    /** Score of this match */
    int score;
    /** match succesful? */
    bool matched;
};

/**
 * @brief simple fuzzy matching of chars in @p pattern with chars in @p str
 * sequentially. If there is a match, it will return true and false otherwise.
 * There is no scoring. You should use this if score is not important for you
 * and only matching is important.
 *
 * @param pattern to search for. For e.g., text entered by a user to filter a
 * list
 * @param str the current string from your list of strings
 * @return true on sucessful match
 *
 * @since 5.79
 */
KCOREADDONS_EXPORT bool matchSimple(QStringView pattern, QStringView str);

/**
 * @brief This should be the function to use if the strings you are matching
 * consists of file names, code etc. @p outscore is the score of this match and
 * should be used to sort the results later. Without sorting of the results this
 * function won't very effective. Also note that this function scores separator
 * matches higher than sequential matches.
 * See @ref matchSequential for more details
 *
 * If you are using this function in a QSortFilterProxyModel based class, keep
 * in mind that during the initial load of the model when the pattern is empty,
 * this function will return false and as a result your "view" will be empty. To
 * prevent this, you should place a check in the beginning of your function
 * which returns true if the pattern is empty.
 *
 * @param pattern to search for. For e.g., text entered by a user to filter a
 * list
 * @param str the current string from your list of strings
 * @param outScore The output score of a particular match. This determines the
 * quality of match and should be used for sorting later.
 * @return A @ref Result type with score of this match and whether there is a match.
 * If there is no match, score is likely useless as the string will be filtered out.
 * If the match is successful, score must be used to sort the results.
 *
 * @since 5.79
 */
KCOREADDONS_EXPORT Result match(QStringView pattern, QStringView str);

/**
 * @brief This is a special case function which scores sequential matches
 * higher than separator matches. For example, if you have the following
 * strings:
 *
 * \code
 * "Sort Items"
 * "Split or join a tag"
 * \endcode
 *
 * and you use @ref match, then it can be that the second result is scored
 * better than the first one despite the fact that the first one is an exact
 * match. This happens because @ref match will score separator matches higher
 * than sequential matches. Use this function if you want sequential matches
 * to take preference. This will be needed if you are filtering user interface
 * strings.
 * to take preference.
 *
 * @param pattern to search for. For e.g., text entered by a user to filter a
 * list
 * @param str the current string from your list of strings
 * @return A @ref Result type with score of this match and whether there is a match.
 * If there is no match, score is likely useless as the string will be filtered out.
 * If the match is successful, score must be used to sort the results.
 *
 * @since 5.79
 */
KCOREADDONS_EXPORT Result matchSequential(QStringView pattern, QStringView str);

/**
 * @brief This is a utility function to display what is being matched.
 *
 * Example:
 * \code
 * pattern = "kate", str = "kateapp" and @p htmlTag = "<b>"
 * output =  <b>k</b><b>a</b><b>t</b><b>e</b>app
 *
 * which will be visible to user as <b>kate</b>app.
 * \endcode
 *
 * @param pattern is the current pattern entered by user
 * @param str is the string that will be wrapped with @p htmlTag.
 * @param htmlTag is the html tag you want to use for example <b> or <span
 * style=...>
 * @param htmlTagClose is the corresponding closing tag for @p htmlTag. The
 * function does not check that whether it is a closing tag for @p htmlTag or
 * not.
 * @return htmlTag wrapped output string. It is the same as @p str
 *
 * @since 5.79
 */
KCOREADDONS_EXPORT QString toFuzzyMatchedDisplayString(QStringView pattern,
                                                       QString str,
                                                       QStringView htmlTag,
                                                       QStringView htmlTagClose);

} // namespace KFuzzyMatcher
