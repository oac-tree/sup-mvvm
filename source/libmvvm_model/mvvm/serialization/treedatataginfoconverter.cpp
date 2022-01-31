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

#include "mvvm/serialization/treedatataginfoconverter.h"

#include "mvvm/model/taginfo.h"
#include "mvvm/serialization/TreeData.h"
#include "mvvm/utils/stringutils.h"

#include <algorithm>
#include <stdexcept>

// ----------------------------------------------------------------------------
// Declarations of constants and helper methods in anonymous namespace.
// ----------------------------------------------------------------------------

namespace
{
const std::string kTagInfoElementType = "TagInfo";
const std::string kMinAttributeKey = "min";
const std::string kMaxAttributeKey = "max";
const std::string kNameAttributeKey = "name";

//! Returns vector of attributes which TreeData object should have.
std::vector<std::string> GetExpectedAttributeKeys();

std::vector<std::string> SplitAndTrim(const std::string &str)
{
  auto result = mvvm::utils::SplitString(str, ",");
  std::transform(result.begin(), result.end(), result.begin(),
                 [](auto s) { return mvvm::utils::TrimWhitespace(s); });
  return result;
}
}  // namespace

// ----------------------------------------------------------------------------
// Implementations declared in the header.
// ----------------------------------------------------------------------------

namespace mvvm
{
bool IsTagInfoConvertible(const TreeData &tree_data)
{
  static const std::vector<std::string> expected_names = GetExpectedAttributeKeys();
  return tree_data.GetType() == kTagInfoElementType
         && expected_names == tree_data.Attributes().GetAttributeNames()
         && tree_data.GetNumberOfChildren() == 0;
}

TagInfo ToTagInfo(const TreeData &tree_data)
{
  if (!IsTagInfoConvertible(tree_data))
  {
    throw std::runtime_error("Error in variant converter: invalid TreeData object.");
  }

  int min = std::stoi(tree_data.GetAttribute(kMinAttributeKey));
  int max = std::stoi(tree_data.GetAttribute(kMaxAttributeKey));
  std::string name = tree_data.GetAttribute(kNameAttributeKey);
  std::vector<std::string> model_types = SplitAndTrim(tree_data.GetContent());
  return TagInfo(name, min, max, model_types);
}

TreeData ToTreeData(const TagInfo &tag_info)
{
  TreeData result(kTagInfoElementType);
  result.AddAttribute(kMinAttributeKey, std::to_string(tag_info.GetMin()));
  result.AddAttribute(kMaxAttributeKey, std::to_string(tag_info.GetMax()));
  result.AddAttribute(kNameAttributeKey, tag_info.GetName());
  result.SetContent(utils::ToCommaSeparatedString(tag_info.GetItemTypes()));
  return result;
}

}  // namespace mvvm

// ----------------------------------------------------------------------------
// Implementation of helper functions from anonymous namespace.
// ----------------------------------------------------------------------------

namespace
{
std::vector<std::string> GetExpectedAttributeKeys()
{
  std::vector<std::string> result = {kMinAttributeKey, kMaxAttributeKey, kNameAttributeKey};
  std::sort(result.begin(), result.end());
  return result;
}

}  // namespace
