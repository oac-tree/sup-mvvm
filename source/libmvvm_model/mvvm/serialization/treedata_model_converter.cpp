/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/serialization/treedata_model_converter.h"

#include <mvvm/interfaces/sessionmodel_interface.h>
#include <mvvm/model/sessionitem.h>
#include <mvvm/model/model_utils.h>
#include <mvvm/serialization/treedata.h>
#include <mvvm/serialization/treedata_item_converter.h>

#include <string>
#include <vector>

namespace
{
const std::string kModelElementType = "Model";
const std::string kTypelAttributeKey = "type";

std::unique_ptr<mvvm::TreeDataItemConverterInterface> CreateConverter(
    const mvvm::ItemFactoryInterface *factory)
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

bool TreeDataModelConverter::IsSessionModelConvertible(const TreeData &tree_data) const
{
  static const std::vector<std::string> expected_attributes({kTypelAttributeKey});

  const bool correct_type = tree_data.GetType() == kModelElementType;
  const bool correct_attributes = tree_data.Attributes().GetAttributeNames() == expected_attributes;

  return correct_type && correct_attributes;
}

std::unique_ptr<TreeData> TreeDataModelConverter::ToTreeData(
    const SessionModelInterface &model) const
{
  auto item_converter = CreateConverter(model.GetFactory());

  auto result = std::make_unique<TreeData>(kModelElementType);

  result->AddAttribute(kTypelAttributeKey, model.GetType());

  for (auto item : model.GetRootItem()->GetAllItems())
  {
    result->AddChild(*item_converter->ToTreeData(*item));
  }

  return result;
}

void TreeDataModelConverter::PopulateSessionModel(const TreeData &tree_data,
                                                  SessionModelInterface &model) const
{
  if (!IsSessionModelConvertible(tree_data))
  {
    throw std::runtime_error("Error in TreeDataModelConverter: inappropriate TreeData");
  }

  if (tree_data.GetAttribute(kTypelAttributeKey) != model.GetType())
  {
    throw std::runtime_error(
        "Error in TreeDataModelConverter: attempt to reconstruct different model type.");
  }

  auto item_converter = CreateConverter(model.GetFactory());

  auto root_item = utils::CreateEmptyRootItem();
  for (const auto &tree_child : tree_data.Children())
  {
    root_item->InsertItem(item_converter->ToSessionItem(tree_child), TagIndex::Append());
  }
  model.Clear(std::move(root_item), nullptr);
}

}  // namespace mvvm
