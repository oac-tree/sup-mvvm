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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "path.h"

#include <algorithm>
#include <iterator>
#include <numeric>
#include <sstream>

namespace mvvm
{
//! Constructs Path object from string containing sequence of integers ("0,0,1,3").

Path Path::CreateFromString(const std::string& str)
{
  Path result;

  std::string str_spaces(str);
  std::replace(str_spaces.begin(), str_spaces.end(), ',', ' ');

  std::istringstream iss(str_spaces);
  (void)std::for_each(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(),
                      [&result](auto x) { result.Append(std::stoi(x)); });
  return result;
}

//! Constructs Path object from vector of integers.

Path Path::CreateFromVector(const std::vector<int>& data)
{
  Path result;
  (void)std::for_each(data.begin(), data.end(), [&result](auto x) { result.Append(x); });
  return result;
}

//! Returns string representing path ("0,0,1,3").

std::string Path::GetString() const
{
  auto comma_fold = [](const std::string& a, int b) { return a + ',' + std::to_string(b); };
  return m_data.empty() ? std::string()
                        : std::accumulate(std::next(m_data.begin()), m_data.end(),
                                          std::to_string(m_data[0]), comma_fold);
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
