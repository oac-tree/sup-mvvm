// ************************************************************************** //
//
//  Operational Applications UI Foundation
//
// ************************************************************************** //

#ifndef MVVM_MODEL_TAGINDEX_H
#define MVVM_MODEL_TAGINDEX_H

#include "mvvm/model_export.h"

#include <string>

namespace ModelView
{
//! Aggregate to hold (tag, index) information for SessionModel.
//! index=-1 is a special value for appending to the end of the in the SessionIteTags context.

class MVVM_MODEL_EXPORT TagIndex
{
public:
  std::string tag = {};
  int index = -1;

  TagIndex() {}

  TagIndex(const std::string& name, int index = -1) : tag(name), index(index) {}
  TagIndex(const char* name, int index = -1) : tag(name), index(index) {}

  TagIndex next() const;

  TagIndex prev() const;

  static TagIndex append(const std::string& tag_name = {});

  static TagIndex prepend(const std::string& tag_name = {});

  bool operator==(const TagIndex& other) const;
  bool operator!=(const TagIndex& other) const;
};

}  // namespace ModelView

#endif  // MVVM_MODEL_TAGINDEX_H
