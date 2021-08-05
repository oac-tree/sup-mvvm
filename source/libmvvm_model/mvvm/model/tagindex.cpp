// ************************************************************************** //
//
//  Operational Applications UI Foundation
//
// ************************************************************************** //

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
