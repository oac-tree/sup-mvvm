// ************************************************************************** //
//
//  Operational Applications UI Foundation
//
// ************************************************************************** //

#include "mvvm/model/taginfo.h"

#include "mvvm/utils/containerutils.h"

#include <sstream>
#include <stdexcept>

namespace ModelView
{
TagInfo::TagInfo() : m_min(0), m_max(-1) {}

TagInfo::TagInfo(std::string name, int min, int max, std::vector<std::string> model_types)
    : m_name(std::move(name)), m_min(min), m_max(max), m_model_types(std::move(model_types))
{
  if (m_min < 0 || (m_min > m_max && m_max >= 0) || m_name.empty())
  {
    std::ostringstream ostr;
    ostr << "Invalid constructor parameters"
         << " " << m_name << " " << m_min << " " << m_max;
    throw std::runtime_error(ostr.str());
  }
}

TagInfo TagInfo::universalTag(std::string name, std::vector<std::string> modelTypes)
{
  return TagInfo(std::move(name), 0, -1, std::move(modelTypes));
}

TagInfo TagInfo::propertyTag(std::string name, std::string model_type)
{
  return TagInfo(std::move(name), 1, 1, {std::move(model_type)});
}

std::string TagInfo::name() const
{
  return m_name;
}

int TagInfo::min() const
{
  return m_min;
}

int TagInfo::max() const
{
  return m_max;
}

std::vector<std::string> TagInfo::modelTypes() const
{
  return m_model_types;
}

//! Returns true if given modelType matches the list of possible model types.

bool TagInfo::isValidChild(const std::string& model_type) const
{
  return m_model_types.empty() ? true : Utils::Contains(m_model_types, model_type);
}

//! Returns true if this tag is used to store single properties.
//! Properties are children that are created in SessionItem constructor using ::addProperty method.

bool TagInfo::isSinglePropertyTag() const
{
  return m_min == 1 && m_max == 1;
}

bool TagInfo::operator==(const TagInfo& other) const
{
  return m_name == other.m_name && m_min == other.m_min && m_max == other.m_max
         && m_model_types == other.m_model_types;
}

bool TagInfo::operator!=(const TagInfo& other) const
{
  return !(*this == other);
}

}  // namespace ModelView
