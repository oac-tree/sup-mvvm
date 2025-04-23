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

#include "tree_data_model_converter.h"

#include "tree_data.h"
#include "tree_data_helper.h"
#include "tree_data_item_converter.h"

#include <mvvm/model/i_session_model.h>
#include <mvvm/model/item_factory.h>
#include <mvvm/model/model_utils.h>
#include <mvvm/model/session_item.h>

#include <string>
#include <vector>

namespace
{
const std::string kModelElementType = "Model";
const std::string kTypelAttributeKey = "type";

std::unique_ptr<mvvm::ITreeDataItemConverter> CreateConverter(const mvvm::IItemFactory *factory)
{
  return std::make_unique<mvvm::TreeDataItemConverter>(factory, mvvm::ConverterMode::kClone);
}

}  // namespace

namespace mvvm
{
struct TreeDataModelConverter::TreeDataModelConverterImpl
{
  ConverterMode m_converter_mode;

  TreeDataModelConverterImpl(ConverterMode converter_mode) : m_converter_mode(converter_mode) {}
};

TreeDataModelConverter::TreeDataModelConverter(ConverterMode converter_mode)
    : p_impl(std::make_unique<TreeDataModelConverterImpl>(converter_mode))
{
}

TreeDataModelConverter::~TreeDataModelConverter() = default;

bool TreeDataModelConverter::IsSessionModelConvertible(const tree_data_t &tree_data) const
{
  static const std::vector<std::string> expected_attributes({kTypelAttributeKey});

  const bool correct_type = tree_data.GetNodeName() == kModelElementType;
  const bool correct_attributes = GetAttributeNames(tree_data) == expected_attributes;

  return correct_type && correct_attributes;
}

std::unique_ptr<tree_data_t> TreeDataModelConverter::ToTreeData(const ISessionModel &model) const
{
  auto item_converter = CreateConverter(&GetGlobalItemFactory());

  auto result = std::make_unique<tree_data_t>(kModelElementType);

  result->AddAttribute(kTypelAttributeKey, model.GetType());

  for (auto item : model.GetRootItem()->GetAllItems())
  {
    result->AddChild(*item_converter->ToTreeData(*item));
  }

  return result;
}

void TreeDataModelConverter::PopulateSessionModel(const tree_data_t &tree_data,
                                                  ISessionModel &model) const
{
  if (!IsSessionModelConvertible(tree_data))
  {
    throw RuntimeException("Error in TreeDataModelConverter: inappropriate TreeData");
  }

  if (tree_data.GetAttribute(kTypelAttributeKey) != model.GetType())
  {
    throw RuntimeException(
        "Error in TreeDataModelConverter: attempt to reconstruct different model type.");
  }

  auto item_converter = CreateConverter(&GetGlobalItemFactory());

  auto root_item = utils::CreateEmptyRootItem();
  for (const auto &tree_child : tree_data.Children())
  {
    root_item->InsertItem(item_converter->ToSessionItem(tree_child), TagIndex::Append());
  }
  model.ReplaceRootItem(std::move(root_item));
}

}  // namespace mvvm
