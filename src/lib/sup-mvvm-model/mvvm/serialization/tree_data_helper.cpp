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

#include "tree_data_helper.h"

#include "tree_data_constants.h"

#include <mvvm/utils/container_utils.h>

#include <sup/xml/tree_data_parser.h>

namespace mvvm
{

std::vector<std::string> GetAttributeNames(const tree_data_t& tree_data)
{
  std::vector<std::string> result;
  for (const auto& [name, _] : tree_data.Attributes())
  {
    result.push_back(name);
  }
  return result;
}

std::unique_ptr<tree_data_t> ParseXMLElementString(const std::string& element)
{
  const std::string header{R"RAW(<?xml version="1.0" encoding="UTF-8"?>
)RAW"};

  const std::string content = header + element;
  return mvvm::xml::TreeDataFromString(content);
}

bool IsSessionModelConvertible(const tree_data_t& tree_data)
{
  static const std::vector<std::string> expected_attributes({constants::kTypeAttributeKey});

  const bool correct_type = tree_data.GetNodeName() == constants::kModelElementType;
  const bool correct_attributes = GetAttributeNames(tree_data) == expected_attributes;

  return correct_type && correct_attributes;
}

bool IsItemContainerConvertible(const tree_data_t& tree_data)
{
  const bool correct_type = tree_data.GetNodeName() == constants::kItemContainerElementType;
  const bool correct_attributes = tree_data.GetNumberOfAttributes() == 0;
  const bool correct_children = tree_data.GetNumberOfChildren() > 0;  // at least TagInfo
  return correct_type && correct_attributes && correct_children;
}

bool IsTaggedItemsConvertible(const tree_data_t& tree_data)
{
  const std::vector<std::string> expected_attributes({constants::kDefaultTagKey});
  const bool correct_type = tree_data.GetNodeName() == constants::kTaggedItemsElementType;
  const bool correct_attributes = GetAttributeNames(tree_data) == expected_attributes;
  return correct_type && correct_attributes;
}

bool IsSessionItemConvertible(const tree_data_t& tree_data)
{
  static const std::vector<std::string> expected_attributes({constants::kTypeAttributeKey});

  const bool correct_type = tree_data.GetNodeName() == constants::kItemElementType;
  const bool correct_attributes = GetAttributeNames(tree_data) == expected_attributes;
  const bool correct_children_count = tree_data.GetNumberOfChildren() == 2;

  return correct_type && correct_attributes && correct_children_count;
}

bool IsSessionItemDataConvertible(const tree_data_t& tree_data)
{
  return tree_data.GetNodeName() == constants::kItemDataElementType
         && tree_data.GetNumberOfAttributes() == 0;
  // there is no sence to require empty content, it might still contains '\n' symbol,
  // depending on the way TreeData was constructed from XML content.
}

bool IsDataRoleConvertible(const tree_data_t& tree_data)
{
  auto attribute_names = GetAttributeNames(tree_data);
  const bool correct_attributes = utils::Contains(attribute_names, constants::kRoleAttributeKey)
                                  && utils::Contains(attribute_names, constants::kTypeAttributeKey);
  return tree_data.GetNodeName() == constants::kVariantElementType && correct_attributes
         && tree_data.GetNumberOfChildren() == 0;
}

}  // namespace mvvm
