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

#include "path.h"

#include <mvvm/utils/string_utils.h>

#include <algorithm>

namespace mvvm
{


Path Path::CreateFromString(const std::string& str)
{
  return Path::CreateFromVector(utils::StringToVector<PathElement>(str));
}

Path Path::CreateFromVector(const std::vector<PathElement>& data)
{
  Path result;
  (void)std::for_each(data.begin(), data.end(),
                      [&result](auto element) { result.Append(element); });
  return result;
}

std::string Path::GetString() const
{
  return utils::VectorToString(m_data);
}

std::size_t Path::GetLength() const
{
  return m_data.size();
}

void Path::Append(Path::PathElement element)
{
  m_data.push_back(element);
}

void Path::Prepend(Path::PathElement element)
{
  (void)m_data.insert(m_data.begin(), element);
}

Path::iterator Path::begin()
{
  return m_data.begin();
}

Path::const_iterator Path::begin() const
{
  return m_data.begin();
}

Path::iterator Path::end()
{
  return m_data.end();
}

Path::const_iterator Path::end() const
{
  return m_data.end();
}

}  // namespace mvvm
