/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "tree_data_taginfo_converter.h"

#include "tree_data.h"  // IWYU pragma: export
#include "tree_data_constants.h"
#include "tree_data_helper.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/model/taginfo.h>
#include <mvvm/utils/container_utils.h>
#include <mvvm/utils/string_utils.h>

#include <algorithm>

namespace mvvm::TreeDataTagInfoConverter
{

namespace
{

//! Returns vector of attributes which TreeData object should have.
std::vector<std::string> GetExpectedAttributeKeys()
{
  std::vector<std::string> result = {constants::kMinAttributeKey, constants::kMaxAttributeKey,
                                     constants::kNameAttributeKey};
  std::sort(result.begin(), result.end());
  return result;
}

std::vector<std::string> SplitAndTrim(const std::string &str)
{
  auto result = mvvm::utils::SplitString(str, ",");
  std::transform(result.begin(), result.end(), result.begin(),
                 [](auto element) { return mvvm::utils::TrimWhitespace(element); });
  return result;
}

bool IsValidAttributes(const std::vector<std::string> &attributes,
                       const std::vector<std::string> &possible_values)
{
  auto predicate = [&possible_values](const auto &element)
  { return mvvm::utils::Contains(possible_values, element); };
  return std::all_of(attributes.begin(), attributes.end(), predicate);
}

}  // namespace

bool IsTagInfoConvertible(const tree_data_t &tree_data)
{
  static const std::vector<std::string> expected_names = GetExpectedAttributeKeys();
  return tree_data.GetNodeName() == constants::kTagInfoElementType
         && IsValidAttributes(GetAttributeNames(tree_data), expected_names)
         && tree_data.GetNumberOfChildren() == 0;
}

TagInfo ToTagInfo(const tree_data_t &tree_data)
{
  if (!IsTagInfoConvertible(tree_data))
  {
    throw RuntimeException("Error in variant converter: invalid TreeData object.");
  }

  std::optional<size_t> min{};
  std::optional<size_t> max{};

  if (tree_data.HasAttribute(constants::kMinAttributeKey))
  {
    min = std::stol(tree_data.GetAttribute(constants::kMinAttributeKey));
  }
  if (tree_data.HasAttribute(constants::kMaxAttributeKey))
  {
    max = std::stol(tree_data.GetAttribute(constants::kMaxAttributeKey));
  }
  const std::string name = tree_data.GetAttribute(constants::kNameAttributeKey);
  const std::vector<std::string> model_types = SplitAndTrim(tree_data.GetContent());
  return {name, min, max, model_types};
}

tree_data_t ToTreeData(const TagInfo &tag_info)
{
  tree_data_t result(constants::kTagInfoElementType);
  if (tag_info.HasMin())
  {
    result.AddAttribute(constants::kMinAttributeKey, std::to_string(tag_info.GetMin()));
  }
  if (tag_info.HasMax())
  {
    result.AddAttribute(constants::kMaxAttributeKey, std::to_string(tag_info.GetMax()));
  }
  result.AddAttribute(constants::kNameAttributeKey, tag_info.GetName());
  result.SetContent(utils::ToCommaSeparatedString(tag_info.GetItemTypes()));
  return result;
}

}  // namespace mvvm::TreeDataTagInfoConverter
