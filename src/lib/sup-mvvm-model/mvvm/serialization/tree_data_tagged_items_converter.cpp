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

#include "tree_data_tagged_items_converter.h"

#include "tree_data.h"
#include "tree_data_helper.h"
#include "tree_data_item_container_converter.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/model/session_item_container.h>
#include <mvvm/model/tagged_items.h>

namespace
{
const std::string kTaggedItemsElementType = "TaggedItems";
const std::string kDefaultTagKey = "defaultTag";

}  // namespace

namespace mvvm
{
struct TreeDataTaggedItemsConverter::TreeDataTaggedItemsConverterImpl
{
  ConverterCallbacks m_callbacks;

  explicit TreeDataTaggedItemsConverterImpl(ConverterCallbacks callbacks)
      : m_callbacks(std::move(callbacks))
  {
  }
};

TreeDataTaggedItemsConverter::TreeDataTaggedItemsConverter(const ConverterCallbacks &callbacks)
    : p_impl(std::make_unique<TreeDataTaggedItemsConverterImpl>(callbacks))
{
}

TreeDataTaggedItemsConverter::~TreeDataTaggedItemsConverter() = default;

bool TreeDataTaggedItemsConverter::IsTaggedItemsConvertible(const tree_data_t &tree_data) const
{
  const std::vector<std::string> expected_attributes({kDefaultTagKey});
  const bool correct_type = tree_data.GetNodeName() == kTaggedItemsElementType;
  const bool correct_attributes = GetAttributeNames(tree_data) == expected_attributes;
  return correct_type && correct_attributes;
}

std::unique_ptr<TaggedItems> TreeDataTaggedItemsConverter::ToTaggedItems(
    const tree_data_t &tree_data) const
{
  auto result = std::make_unique<TaggedItems>();
  PopulateTaggedItems(tree_data, *result);
  return result;
}

void TreeDataTaggedItemsConverter::PopulateTaggedItems(const tree_data_t &tree_data,
                                                       TaggedItems &tagged_items) const
{
  if (!IsTaggedItemsConvertible(tree_data))
  {
    throw RuntimeException("Error in TreeDataTaggedItemsConverter: uncompatible TreeData");
  }

  tagged_items.SetDefaultTag(tree_data.GetAttribute(kDefaultTagKey));

  for (const auto &child_data : tree_data.Children())
  {
    auto container =
        ContainerConverter::ToSessionItemContainer(child_data, p_impl->m_callbacks.create_item);
    tagged_items.AppendContainer(std::move(container));
  }
}

std::unique_ptr<tree_data_t> TreeDataTaggedItemsConverter::ToTreeData(
    const TaggedItems &tagged_items) const
{
  auto result = std::make_unique<tree_data_t>(kTaggedItemsElementType);
  result->AddAttribute(kDefaultTagKey, tagged_items.GetDefaultTag());
  for (auto &container : tagged_items)
  {
    result->AddChild(*ContainerConverter::ToTreeData(
        *container, p_impl->m_callbacks.create_treedata, p_impl->m_callbacks.filter_item));
  }

  return result;
}

}  // namespace mvvm
