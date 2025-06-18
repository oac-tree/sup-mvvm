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

#include "session_item_composer_components.h"

#include "tree_data_constants.h"
#include "tree_data_helper.h"
#include "tree_data_itemdata_converter.h"
#include "tree_data_tagged_items_converter.h"
#include "tree_data_taginfo_converter.h"

#include <mvvm/core/unique_id_generator.h>
#include <mvvm/model/i_item_factory.h>
#include <mvvm/model/session_item.h>
#include <mvvm/model/session_item_container.h>
#include <mvvm/model/session_item_data.h>
#include <mvvm/model/tagged_items.h>

namespace mvvm
{

// ---------------------------------------------------------------------------------------------------
// SessionItemComposerComponent
// ---------------------------------------------------------------------------------------------------

bool ItemComposerComponent::HasNextComponent() const
{
  return !m_component_queue.empty();
}

std::size_t ItemComposerComponent::GetComponentCount() const
{
  return m_component_queue.size();
}

void ItemComposerComponent::AddNextComponent(component_t component)
{
  m_component_queue.emplace_back(std::move(component));
}

ItemComposerComponent::component_t ItemComposerComponent::TakeNextComponent()
{
  auto popped = std::move(m_component_queue.front());
  m_component_queue.pop_front();
  return popped;
}

// ---------------------------------------------------------------------------------------------------
// ItemContainerFromTreeData
// ---------------------------------------------------------------------------------------------------

ItemContainerFromTreeData::ItemContainerFromTreeData(const ItemComposerContext &context,
                                                     const tree_data_t &tree_data,
                                                     TaggedItems &tagged_items)
{
  if (!IsItemContainerConvertible(tree_data))
  {
    throw RuntimeException("Incompatible TreeData");
  }

  auto tag_info = ToTagInfo(tree_data.Children().at(0));
  auto container = std::make_unique<SessionItemContainer>(tag_info);
  auto container_ptr = container.get();

  tagged_items.AppendContainer(std::move(container));

  // first child in TreeData corresponds to TagInfo which has been already processed
  for (auto it = std::next(tree_data.Children().begin()); it != tree_data.Children().end(); ++it)
  {
    auto model_type = (*it).GetAttribute(constants::kTypelAttributeKey);

    auto child = context.factory->CreateItem(model_type);
    auto child_ptr = child.get();
    container_ptr->InsertItem(std::move(child), container_ptr->GetItemCount());

    AddNextComponent(std::make_unique<SessionItemFromTreeData>(context, (*it), *child_ptr));
  }
}

// ---------------------------------------------------------------------------------------------------
// TaggedItemsFromTreeData
// ---------------------------------------------------------------------------------------------------

TaggedItemsFromTreeData::TaggedItemsFromTreeData(const ItemComposerContext &context,
                                                 const tree_data_t &tree_data,
                                                 TaggedItems &tagged_items)
{
  if (!IsTaggedItemsConvertible(tree_data))
  {
    throw RuntimeException("Incompatible TreeData");
  }

  tagged_items.SetDefaultTag(tree_data.GetAttribute(constants::kDefaultTagKey));

  for (const auto &container_data : tree_data.Children())
  {
    AddNextComponent(
        std::make_unique<ItemContainerFromTreeData>(context, container_data, tagged_items));
  }
}

// ---------------------------------------------------------------------------------------------------
// SessionItemFromTreeData
// ---------------------------------------------------------------------------------------------------
SessionItemFromTreeData::SessionItemFromTreeData(const ItemComposerContext &context,
                                                 const tree_data_t &tree_data, SessionItem &item)
{
  if (!IsSessionItemConvertible(tree_data))
  {
    throw RuntimeException("Incompatible TreeData");
  }

  auto item_type = tree_data.GetAttribute(constants::kTypelAttributeKey);

  if (item_type != item.GetType())
  {
    throw RuntimeException("Item model mismatch");
  }

  const TreeDataItemDataConverter data_converter;
  data_converter.PopulateItemData(tree_data.Children().at(0), *item.GetItemData());

  if (IsRegenerateIdWhenBackFromXML(context.mode))
  {
    item.SetData(UniqueIdGenerator::Generate(), DataRole::kIdentifier);
  }

  AddNextComponent(std::make_unique<TaggedItemsFromTreeData>(context, tree_data.Children().at(1),
                                                             *item.GetTaggedItems()));
}

}  // namespace mvvm
