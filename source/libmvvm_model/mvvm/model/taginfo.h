// ************************************************************************** //
//
//  Operational Applications UI Foundation
//
// ************************************************************************** //

#ifndef MVVM_MODEL_TAGINFO_H
#define MVVM_MODEL_TAGINFO_H

#include <string>
#include <vector>

namespace ModelView
{
//! Holds info about the single tag for SessionItem.
//! The tag specifies information about children that can be added to a SessionItem. A tag has a
//! name, min, max allowed number of children, and vector of all modelTypes that children can have.

class TagInfo
{
public:
  TagInfo();

  TagInfo(std::string name, int min, int max, std::vector<std::string> model_types);

  //! Constructs universal tag intended for unlimited amount of various items.
  static TagInfo universalTag(std::string name, std::vector<std::string> model_types = {});

  //! Constructs tag intended for single property.
  static TagInfo propertyTag(std::string name, std::string model_type);

  std::string name() const;

  int min() const;

  int max() const;

  std::vector<std::string> modelTypes() const;

  bool maximumReached() const;

  bool isValidChild(const std::string& model_type) const;

  bool isSinglePropertyTag() const;

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
