/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
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

#ifndef MVVM_MODEL_TAGINFO_H
#define MVVM_MODEL_TAGINFO_H

#include "mvvm/model_export.h"

#include <string>
#include <vector>

namespace ModelView
{
//! Holds info about the single tag for SessionItem.
//! The tag specifies information about children that can be added to a SessionItem. A tag has a
//! name, min, max allowed number of children, and vector of all types that children can have.

class MVVM_MODEL_EXPORT TagInfo
{
public:
  TagInfo();

  TagInfo(std::string name, int min, int max, std::vector<std::string> item_types);

  static TagInfo CreateUniversalTag(std::string name, std::vector<std::string> item_types = {});

  static TagInfo CreatePropertyTag(std::string name, std::string item_type);

  std::string GetName() const;

  int GetMin() const;

  int GetMax() const;

  std::vector<std::string> GetItemTypes() const;

  bool IsValidType(const std::string& item_type) const;

  bool IsSinglePropertyTag() const;

  bool operator==(const TagInfo& other) const;
  bool operator!=(const TagInfo& other) const;

private:
  std::string m_name;                     //!< the name of the tag
  int m_min;                              //!< minimum allowed number of items in a tag
  int m_max;                              //!< maximum allowed number of items in a tag
  std::vector<std::string> m_item_types;  //!< vector of allowed item types
};

}  // namespace ModelView

#endif  // MVVM_MODEL_TAGINFO_H
