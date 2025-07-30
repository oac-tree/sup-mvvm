/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_UTILS_STRING_UTILS_H_
#define MVVM_UTILS_STRING_UTILS_H_

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/model_export.h>

#include <optional>
#include <sstream>
#include <string>
#include <vector>
#include <cstdint>

namespace mvvm::utils
{

const std::string kDefaultSeparator = ",";

/**
 * @brief Returns string representation of double with given precision.
 *
 * Provides additional formatting on top of iomanip, so "double x{0}" becomes "0.0".
 *
 * @param input The value to convert to a string.
 * @param precision Number of digits after ddecimal sign.
 * @return Resulting string.
 */
MVVM_MODEL_EXPORT std::string DoubleToString(double input, std::int32_t precision = 12);

/**
 * @brief Returns string representation of scientific double.
 *
 * Provides additional formatting on top of iomanip, so "double x{1}" becomes "1.0e+00".
 *
 * @param input The value to convert to a string.
 * @param precision Number of digits after ddecimal sign.
 * @return Resulting string.
 */
MVVM_MODEL_EXPORT std::string ScientificDoubleToString(double input, std::int32_t precision = 6);

/**
 * @brief Returns string after trimming whitespace surrounding, including tabs and carriage returns.
 */
MVVM_MODEL_EXPORT std::string TrimWhitespace(const std::string& str);

/**
 * @brief Converts string to bool.
 *
 * String should represent one of "false", "true", "False", etc. Will throw if it is not the case.
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
 * Requires that the string represents exactly one double and contains no other literals. Empty
 * spaces at the beginning and end of the string are still allowed.
 *
 * @param str A string to convert.
 * @return Optional double result of the conversion.
 */
MVVM_MODEL_EXPORT std::optional<double> StringToDouble(const std::string& str);

/**
 * @brief Converts string to integer.
 *
 * Requires that the string represents exactly one integer and no extra symbols are defined. Empty
 * spaces at the beginning and end of the string are still allowed.
 *
 * @param str A string to convert.
 * @return Optional integer result of the conversion.
 */
MVVM_MODEL_EXPORT std::optional<std::int32_t> StringToInteger(const std::string& str);

/**
 * @brief Split string on substring using given delimiter.
 *
 * Reproduces Python's str.split() behavior.
 *
 * @param str A string to split.
 * @param delimiter A delimiter.
 * @return Vector of string.
 */
MVVM_MODEL_EXPORT std::vector<std::string> SplitString(const std::string& str,
                                                       const std::string& delimiter);

/**
 * @brief Parses string for double values and returns result as a vector.
 *
 * @details All non-numeric symbols are ingored.
 */
MVVM_MODEL_EXPORT std::vector<double> ParseSpaceSeparatedDoubles(const std::string& str);

/**
 * @brief Replaces substring in a string and returns result to the user.
 *
 * @param str Original string.
 * @param substr A substring to replace.
 * @param replacement New substring.
 * @return Resulting string.
 */
MVVM_MODEL_EXPORT std::string ReplaceSubString(const std::string& str, const std::string& substr,
                                               const std::string& replacement);

/**
 * @brief Returns content of key in a string.
 *
 * For text="data[value1] config[setting] another[item]" and key_name="data" will return "value1"
 */
std::optional<std::string> GetKeyContent(const std::string& text, const std::string& key_name);

/**
 * @brief Converts vector of elements to a string using given delimiter.
 *
 * @tparam ElementT The type of vector element.
 * @param vec Vector of items
 * @param delimiter The delimiter
 * @return Resulting string.
 *
 * @details Function uses custom converter for double values to provide nicer look.
 */
template <typename ElementT>
std::string VectorToString(const std::vector<ElementT>& vec,
                           const std::string& delimiter = kDefaultSeparator)
{
  if (vec.empty())
  {
    return {};
  }

  std::ostringstream oss;
  for (size_t i = 0; i < vec.size(); ++i)
  {
    if constexpr (std::is_same_v<ElementT, double>)
    {
      oss << DoubleToString(vec[i]);
    }
    else
    {
      oss << vec[i];
    }

    if (i != vec.size() - 1)
    {
      oss << delimiter;
    }
  }
  return oss.str();
}

/**
 * @brief Split string and convert content to the vector of given types.
 *
 * @param str The input string
 * @param delimiter
 * @return Vector of elements
 *
 * @details  Will throw for malformed strings. Good: "1.0, 2.0, 3.0". Bad: ", 2.0, 3.0" or
 * "1.0a, 2.0, 3.0" or "1.0, 2.0, 3.0, "
 */
template <typename ElementT>
std::vector<ElementT> StringToVector(const std::string& str,
                                     const std::string& delimiter = kDefaultSeparator)
{
  std::vector<ElementT> result;

  auto parts = SplitString(str, delimiter);
  result.reserve(parts.size());
  for (const auto& part : parts)
  {
    std::istringstream iss(part);
    iss.imbue(std::locale::classic());
    ElementT value;
    iss >> value;
    if (!iss.fail() && iss.eof())
    {
      result.push_back(value);
    }
    else
    {
      throw RuntimeException("Conversion failed for: " + str);
    }
  }

  return result;
}

}  // namespace mvvm::utils

#endif  // MVVM_UTILS_STRING_UTILS_H_
