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

#include "string_format.h"

#include <mvvm/utils/string_utils.h>

namespace
{

/**
 * @brief Returns a string representing a pattern to replace for given argument count.
 *
 * @details 0 -> "%1", 1 -> "%2"
 */
std::string GetArgString(size_t arg_count)
{
  return "%" + std::to_string(arg_count + 1);
}

}  // namespace

namespace mvvm::utils
{

StringFormat::StringFormat(std::string str) : m_value(std::move(str)) {}

StringFormat &StringFormat::arg(const std::string &replacement)
{
  m_value = ReplaceSubString(m_value, GetArgString(m_arg_count), replacement);
  m_arg_count++;
  return *this;
}

StringFormat::operator std::string()
{
  return m_value;
}

}  // namespace mvvm::utils
