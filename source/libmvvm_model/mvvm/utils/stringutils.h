/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_UTILS_STRINGUTILS_H
#define MVVM_UTILS_STRINGUTILS_H

#include <string>
#include <optional>
#include <vector>
#include "mvvm/model_export.h"

namespace ModelView::Utils
{

//! Returns string representation of double with given precision.
//! Provides additional formatting on top of iomanip, so "double x{0}" becomes "0.0".
MVVM_MODEL_EXPORT std::string DoubleToString(double input, int precision = 12);

//! Returns string representation of scientific double.
//! Provides additional formatting on top of iomanip, so "double x{1}" becomes "1.0e+00".
MVVM_MODEL_EXPORT std::string ScientificDoubleToString(double input, int precision = 6);

//! Returns string after trimming whitespace surrounding, including tabs and carriage returns.
MVVM_MODEL_EXPORT std::string TrimWhitespace(const std::string& str);

//! Converts string to bool. String should represent one of "false", "true", "False", etc.
//! Will throw if it is not the case.
MVVM_MODEL_EXPORT bool StringToBool(std::string str);

//! Converts bool to string.
MVVM_MODEL_EXPORT std::string FromBool(bool value);

//! Converts string to double value using classic locale and returns it in the form of optional.
//! Requires that string represents exactly one double and contains no other literals. Empty
//! spaces at the beginning and end of the string are still allowed.
MVVM_MODEL_EXPORT std::optional<double> StringToDouble(const std::string& str);

//! Converts string to integer. Requires that string represents exactly one integer and
//! no extra symbols are defined. Empty spaces at the beginning and end of the string are still
//! allowed.
MVVM_MODEL_EXPORT std::optional<int> StringToInteger(const std::string& str);

//! Split string on substring using given delimeter. Reproduces Python's str.split() behavior.
MVVM_MODEL_EXPORT std::vector<std::string> SplitString(const std::string& str,
                                                       const std::string& delimeter);

//! Parses string for double values and returns result as a vector.
//! All non-numeric symbols are ingored.
MVVM_MODEL_EXPORT std::vector<double> ParseSpaceSeparatedDoubles(const std::string& str);

//! Parses string for double values and stores result in a vector.
//! All non-numeric symbols are ingored.
MVVM_MODEL_EXPORT void ParseSpaceSeparatedDoubles(const std::string& str,
                                                  std::vector<double>& result);

//! Parses string of comma separated double values and returns result as a vector.
//! Will throw for malformed strings.
//! Good: "1.0, 2.0, 3.0"
//! Bad: ", 2.0, 3.0" or "1.0a, 2.0, 3.0" or "1.0, 2.0, 3.0, "
MVVM_MODEL_EXPORT std::vector<double> ParseCommaSeparatedDoubles(const std::string& str);

//! Converts vector of doubles to comma separated string.
MVVM_MODEL_EXPORT std::string ToCommaSeparatedString(const std::vector<double>& vec);

//! Converts vector of strings to comma separated string.
MVVM_MODEL_EXPORT std::string ToCommaSeparatedString(const std::vector<std::string>& vec);

}

#endif
