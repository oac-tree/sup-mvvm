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

#ifndef MVVM_UTILS_STRING_FORMAT_H_
#define MVVM_UTILS_STRING_FORMAT_H_

#include <mvvm/model_export.h>

#include <string>

namespace mvvm::utils
{

/**
 * @brief The StringFormat class provides very basic string formating.
 *
 *
 * @details It tries to mimic QString("%1").arg(42), like in the following:
 *
 * @code{.cpp}
 * const std::string str("<Value>%1, %2</Value>");
 * std::string result = StringFormat(str).arg("42").arg("43");
 * assert(result == "<Value>42, 43</Value>);
 * @endcode
 */
class StringFormat
{
public:
  explicit StringFormat(std::string str);

  StringFormat(const StringFormat& other) = delete;
  StringFormat& operator=(const StringFormat& other) = delete;

  /**
   * @brief Replaces pattern in internal buffer.
   *
   * @param replacement The value to use for replacement.
   * @return Returns self for consequence calls.
   *
   * @details The result depends on number of precedent calls. If this.arg() is called for the first
   * time, it will replace pattern "%1" with the given replacement string. The second call will look
   * for "%2", etc.
   */
  StringFormat& arg(const std::string& replacement);

  /**
   * @brief Cast operator to construct strings.
   */
  operator std::string();

private:
  std::string m_value;
  int m_arg_count{0};
};

}  // namespace mvvm::utils

#endif  // MVVM_UTILS_STRING_FORMAT_H_
