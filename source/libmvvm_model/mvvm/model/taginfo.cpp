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

//! Constructs universal tag intended for unlimited amount of various items.

TagInfo TagInfo::CreateUniversalTag(std::string name, std::vector<std::string> modelTypes)
{
  return TagInfo(std::move(name), 0, -1, std::move(modelTypes));
}

//! Constructs tag intended for single property.

TagInfo TagInfo::CreatePropertyTag(std::string name, std::string model_type)
{
  return TagInfo(std::move(name), 1, 1, {std::move(model_type)});
}

//! Returns the name of this class.

std::string TagInfo::GetName() const
{
  return m_name;
}

//! Returns minimum allowed amount of items with this tag.

int TagInfo::GetMin() const
{
  return m_min;
}

//! Returns maximum allowed amount of items with this tag.

int TagInfo::GetMax() const
{
  return m_max;
}

//! Returns vector of allowed model types which this tag accepts.

std::vector<std::string> TagInfo::GetModelTypes() const
{
  return m_model_types;
}

//! Returns true if given modelType matches the list of possible model types.

bool TagInfo::IsValidChild(const std::string& model_type) const
{
  return m_model_types.empty() ? true : Utils::Contains(m_model_types, model_type);
}

//! Returns true if this tag is used to store single properties.
//! Properties are children that are created in SessionItem constructor using ::addProperty method.

bool TagInfo::IsSinglePropertyTag() const
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
