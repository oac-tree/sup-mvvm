/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_UTILS_STRING_UTILS_H_
#define MVVM_UTILS_STRING_UTILS_H_

#include <mvvm/model_export.h>

#include <optional>
#include <string>
#include <vector>

namespace mvvm::utils
{

/**
 * @brief Returns string representation of double with given precision.
 *
 * @param input The value to convert to a string.
 * @param precision Number of digits after ddecimal sign.
 * @return String
 *
 * @details Provides additional formatting on top of iomanip, so "double x{0}" becomes "0.0".
 */
MVVM_MODEL_EXPORT std::string DoubleToString(double input, int precision = 12);

/**
 * @brief Returns string representation of scientific double.
 *
 * @param input The value to convert to a string.
 * @param precision Number of digits after ddecimal sign.
 * @return String
 *
 * @details Provides additional formatting on top of iomanip, so "double x{1}" becomes "1.0e+00".
 */
MVVM_MODEL_EXPORT std::string ScientificDoubleToString(double input, int precision = 6);

/**
 * @brief Returns string after trimming whitespace surrounding, including tabs and carriage returns.
 */
MVVM_MODEL_EXPORT std::string TrimWhitespace(const std::string& str);

/**
 * @brief Converts string to bool.
 *
 * @details Converts string to bool. String should represent one of "false", "true", "False", etc.
 * Will throw if it is not the case.
 *
 */
MVVM_MODEL_EXPORT bool StringToBool(const std::string& str);

/**
 * @brief Converts bool to string.
 */
MVVM_MODEL_EXPORT std::string FromBool(bool value);

/**
 * @brief Converts string to double value using classic locale and returns it in the form of
 * optional.
 *
 * @param str A string to convert.
 * @return Optional double result of the conversion.
 *
 * @details Requires that string represents exactly one double and contains no other literals. Empty
 * spaces at the beginning and end of the string are still allowed.
 */
MVVM_MODEL_EXPORT std::optional<double> StringToDouble(const std::string& str);

/**
 * @brief Converts string to integer.
 *
 * @param str A string to convert.
 * @return Optional integer result of the conversion.
 *
 * @details Requires that string represents exactly one integer and no extra symbols are defined.
 * Empty spaces at the beginning and end of the string are still allowed.
 */
MVVM_MODEL_EXPORT std::optional<int> StringToInteger(const std::string& str);

/**
 * @brief Split string on substring using given delimeter.
 *
 * @param str A string to split.
 * @param delimeter A delimeter.
 *
 * @return Vector of string.
 *
 * @details Reproduces Python's str.split() behavior.
 */
MVVM_MODEL_EXPORT std::vector<std::string> SplitString(const std::string& str,
                                                       const std::string& delimeter);

/**
 * @brief Parses string for double values and returns result as a vector.
 *
 * @details All non-numeric symbols are ingored.
 */
MVVM_MODEL_EXPORT std::vector<double> ParseSpaceSeparatedDoubles(const std::string& str);

/**
 * @brief Parses string for double values and stores result in a vector.
 *
 * @details All non-numeric symbols are ingored.
 */
MVVM_MODEL_EXPORT void ParseSpaceSeparatedDoubles(const std::string& str,
                                                  std::vector<double>& result);

/**
 * @brief Parses string of comma separated double values and returns result as a vector.
 *
 * @details Will throw for malformed strings. Good: "1.0, 2.0, 3.0". Bad: ", 2.0, 3.0" or
 * "1.0a, 2.0, 3.0" or "1.0, 2.0, 3.0, "
 */
MVVM_MODEL_EXPORT std::vector<double> ParseCommaSeparatedDoubles(const std::string& str);

/**
 * @brief Converts vector of doubles to comma separated string.
 */
MVVM_MODEL_EXPORT std::string ToCommaSeparatedString(const std::vector<double>& vec);

/**
 * @brief Converts vector of strings to comma separated string.
 */
MVVM_MODEL_EXPORT std::string ToCommaSeparatedString(const std::vector<std::string>& vec);

/**
 * @brief Replaces substring in a string and returns result to the user.
 *
 * @param str Original string.
 * @param substr A substring to replace.
 * @param replacement New substring.
 *
 * @return Resulting string.
 */
MVVM_MODEL_EXPORT std::string ReplaceSubString(const std::string& str, const std::string& substr,
                                               const std::string& replacement);

}  // namespace mvvm::utils

#endif  // MVVM_UTILS_STRING_UTILS_H_
