// ************************************************************************** //
//
//  Operational Applications UI Foundation
//
// ************************************************************************** //

#ifndef MVVM_MODEL_TAGINFO_H
#define MVVM_MODEL_TAGINFO_H

#include "mvvm/model_export.h"

#include <string>
#include <vector>

namespace ModelView
{
//! Holds info about the single tag for SessionItem.
//! The tag specifies information about children that can be added to a SessionItem. A tag has a
//! name, min, max allowed number of children, and vector of all modelTypes that children can have.

class MVVM_MODEL_EXPORT TagInfo
{
public:
  TagInfo();

  TagInfo(std::string name, int min, int max, std::vector<std::string> model_types);

  static TagInfo CreateUniversalTag(std::string name, std::vector<std::string> model_types = {});

  static TagInfo CreatePropertyTag(std::string name, std::string model_type);

  std::string GetName() const;

  int GetMin() const;

  int GetMax() const;

  std::vector<std::string> GetModelTypes() const;

  bool IsMaximumReached() const;

  bool IsValidChild(const std::string& model_type) const;

  bool IsSinglePropertyTag() const;

  bool operator==(const TagInfo& other) const;
  bool operator!=(const TagInfo& other) const;

private:
  std::string m_name;                      //!< the name of the tag
  int m_min;                               //!< minimum allowed number of items in tag
  int m_max;                               //!< maximum allowed number of items in tag
  std::vector<std::string> m_model_types;  //!< vector of allowed modelTypes
};

}  // namespace ModelView

#endif  // MVVM_MODEL_TAGINFO_H
