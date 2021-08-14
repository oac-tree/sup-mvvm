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

#include "mvvm/serialization/treedatamodelconverter.h"

#include "mvvm/model/sessionmodel.h"
#include "mvvm/serialization/TreeData.h"
#include "mvvm/serialization/treedataitemconverter.h"

#include <string>
#include <vector>

namespace
{
const std::string kModelElementType = "Model";
const std::string kTypelAttributeKey = "type";

std::unique_ptr<ModelView::TreeDataItemConverterInterface> CreateConverter(
    const ModelView::ItemFactoryInterface *factory)
{
  return std::make_unique<ModelView::TreeDataItemConverter>(factory);
}

}  // namespace

namespace ModelView
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

std::unique_ptr<TreeData> TreeDataModelConverter::ToTreeData(const SessionModel &model) const
{
  auto item_converter = CreateConverter(model.factory());

  auto result = std::make_unique<TreeData>(kModelElementType);

  result->AddAttribute(kTypelAttributeKey, model.modelType());

  for (auto item : model.rootItem()->children())
    result->AddChild(*item_converter->ToTreeData(*item));

  return result;
}

void TreeDataModelConverter::PopulateSessionModel(const TreeData &tree_data,
                                                  SessionModel &model) const
{
  if (!IsSessionModelConvertible(tree_data))
    throw std::runtime_error("Error in TreeDataModelConverter: inappropriate TreeData");

  if (tree_data.GetAttribute(kTypelAttributeKey) != model.modelType())
    throw std::runtime_error(
        "Error in TreeDataModelConverter: attempt to reconstruct different model type.");

  auto item_converter = CreateConverter(model.factory());

  auto rebuild_root = [&tree_data, &item_converter](auto parent)
  {
    for (const auto &tree_child : tree_data.Children())
    {
      parent->insertItem(item_converter->ToSessionItem(tree_child), TagIndex::Append());
    }
  };
  model.clear(rebuild_root);
}

}  // namespace ModelView
