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

#include "tree_data_item_converter.h"

#include "converter_types.h"
#include "session_item_composer.h"
#include "tree_data.h"
#include "tree_data_composer.h"
#include "tree_data_constants.h"
#include "tree_data_helper.h"
#include "tree_data_itemdata_converter.h"
#include "tree_data_tagged_items_converter.h"

#include <mvvm/core/unique_id_generator.h>
#include <mvvm/model/i_item_factory.h>
#include <mvvm/model/session_item.h>
#include <mvvm/model/session_item_data.h>
#include <mvvm/model/tagged_items.h>

namespace mvvm
{
struct TreeDataItemConverter::TreeDataItemConverterImpl
{
  TreeDataItemConverter* m_self{nullptr};
  const IItemFactory* m_factory{nullptr};
  ConverterMode m_mode{ConverterMode::kNone};
  std::unique_ptr<TreeDataItemDataConverter> m_itemdata_converter;
  std::unique_ptr<TreeDataTaggedItemsConverter> m_taggedtems_converter;
  std::function<bool(const SessionItem&)> m_filter;

  TreeDataItemConverterImpl(TreeDataItemConverter* self, const IItemFactory* factory,
                            ConverterMode mode, std::function<bool(const SessionItem&)> filter)
      : m_self(self), m_factory(factory), m_mode(mode), m_filter(filter)
  {
    ConverterCallbacks callbacks;

    callbacks.create_treedata = [this](const SessionItem& item)
    { return m_self->ToTreeData(item); };

    callbacks.create_item = [this](const tree_data_t& tree_data)
    { return m_self->ToSessionItem(tree_data); };

    callbacks.update_item = [this](const tree_data_t& tree_data, SessionItem& item)
    { populate_item(tree_data, item); };

    callbacks.item_filter = filter;

    m_itemdata_converter = std::make_unique<TreeDataItemDataConverter>();
    m_taggedtems_converter = std::make_unique<TreeDataTaggedItemsConverter>(callbacks);
  };

  void populate_item(const tree_data_t& tree_data, SessionItem& item)
  {
    auto item_type = tree_data.GetAttribute(constants::kTypelAttributeKey);

    if (item_type != item.GetType())
    {
      throw RuntimeException("Error in TreeDataItemConverter: item model mismatch");
    }

    auto item_data = m_itemdata_converter->ToSessionItemData(tree_data.Children().at(0));
    auto tagged_items = m_taggedtems_converter->ToTaggedItems(tree_data.Children().at(1));

    item.SetDataAndTags(std::move(item_data), std::move(tagged_items));

    // FIXME implement SessionItemData and SessionItemTags update instead of full rebuild
    //    populate_item_data(json[JsonItemFormatAssistant::itemDataKey].toArray(),
    //    *item.itemData());
    //    populate_item_tags(json[JsonItemFormatAssistant::itemTagsKey].toObject(),
    //    *item.itemTags());

    for (auto child : item.GetAllItems())
    {
      child->SetParent(&item);
    }

    if (IsRegenerateIdWhenBackFromXML(m_mode))
    {
      item.SetData(UniqueIdGenerator::Generate(), DataRole::kIdentifier);
    }
  }
};

TreeDataItemConverter::TreeDataItemConverter(
    const IItemFactory* factory, ConverterMode mode,
    const std::function<bool(const SessionItem&)>& filter_func)
    : p_impl(std::make_unique<TreeDataItemConverterImpl>(this, factory, mode, filter_func))
{
}

TreeDataItemConverter::~TreeDataItemConverter() = default;

std::unique_ptr<SessionItem> TreeDataItemConverter::ToSessionItem(
    const tree_data_t& tree_data) const
{
  // if (!IsSessionItemConvertible(tree_data))
  // {
  //   throw RuntimeException("Error in TreeDataItemConverter: uncompatible TreeData");
  // }

  // auto model_type = tree_data.GetAttribute(constants::kTypelAttributeKey);
  // auto result = p_impl->m_factory->CreateItem(model_type);

  // p_impl->populate_item(tree_data, *result);
  // return result;

  SessionItemComposer composer({p_impl->m_factory, p_impl->m_mode});
  return composer.CreateSessionItem(tree_data);
}

std::unique_ptr<tree_data_t> TreeDataItemConverter::ToTreeData(const SessionItem& item) const
{
  const TreeDataComposer composer(p_impl->m_filter);
  return composer.CreateTreeData(item);
}

}  // namespace mvvm
