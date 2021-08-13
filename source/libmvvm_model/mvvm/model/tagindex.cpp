/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Authors       : Gennady Pospelov (IO)
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

#include "mvvm/model/tagindex.h"

namespace ModelView
{
//! Constructs new TagIndex representing next index in given tag.
//! No validity check.

TagIndex TagIndex::next() const
{
  return {tag, index + 1};
}

//! Constructs new TagIndex representing previous index in given tag.
//! No validity check.

TagIndex TagIndex::prev() const
{
  return {tag, index - 1};
}

//! Returns TagIndex corresponding to the append to tag_name.
//! If tag_name =="" the default name will be used in SessionItemTags context.

TagIndex TagIndex::append(const std::string& tag_name)
{
  return {tag_name, -1};
}

//! Returns TagIndex corresponding to prepending to tag_name.
//! If tag_name =="" the default name will be used in SessionItemTags context.

TagIndex TagIndex::prepend(const std::string& tag_name)
{
  return {tag_name, 0};
}

bool TagIndex::operator==(const TagIndex& other) const
{
  return index == other.index && tag == other.tag;
}

bool TagIndex::operator!=(const TagIndex& other) const
{
  return !(*this == other);
}

}  // namespace ModelView
