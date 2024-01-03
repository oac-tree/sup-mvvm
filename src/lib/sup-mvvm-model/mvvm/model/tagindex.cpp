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

#include "tagindex.h"

namespace mvvm
{

TagIndex TagIndex::Next() const
{
  return {tag, index + 1};
}

TagIndex TagIndex::Prev() const
{
  return {tag, index - 1};
}

TagIndex TagIndex::Append(const std::string& tag_name)
{
  return {tag_name, -1};
}

TagIndex TagIndex::First(const std::string& tag_name)
{
  return {tag_name, 0};
}

TagIndex TagIndex::Default(int index)
{
  return {kDefaultTag, index};
}

bool TagIndex::operator==(const TagIndex& other) const
{
  return index == other.index && tag == other.tag;
}

bool TagIndex::operator!=(const TagIndex& other) const
{
  return !(*this == other);
}

}  // namespace mvvm
