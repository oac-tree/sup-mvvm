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

#include "standard_tree_data_composer_components.h"

#include "tree_data_constants.h"
#include "tree_data_itemdata_converter.h"
#include "tree_data_taginfo_converter.h"

#include <mvvm/model/session_item.h>
#include <mvvm/model/session_item_container.h>
#include <mvvm/model/tagged_items.h>

namespace mvvm
{

TreeDataFromItemContainer::TreeDataFromItemContainer(const SessionItemContainer &container,
                                                     const item_filter_t &item_filter)
    : TreeDataComposerComponent(constants::kItemContainerElementType)
{
  AddChild(ToTreeData(container.GetTagInfo()));

  for (const auto &item : container)
  {
    if (item_filter && !item_filter(*item))
    {
      continue;  // skipping item from serialization
    }

    AddNextComponent(std::make_unique<TreeDataFromSessionItem>(*item, item_filter));
  }
}

TreeDataFromTaggedItems::TreeDataFromTaggedItems(const TaggedItems &tagged_items,
                                                 const item_filter_t &item_filter)
    : TreeDataComposerComponent(constants::kTaggedItemsElementType)
{
  AddAttribute(constants::kDefaultTagKey, tagged_items.GetDefaultTag());

  for (auto &container : tagged_items)
  {
    AddNextComponent(std::make_unique<TreeDataFromItemContainer>(*container, item_filter));
  }
}

TreeDataFromSessionItem::TreeDataFromSessionItem(const SessionItem &item,
                                                 const item_filter_t &item_filter)
    : TreeDataComposerComponent(constants::kItemElementType)
{
  AddAttribute(constants::kTypelAttributeKey, item.GetType());

  AddChild(*TreeDataItemDataConverter::ToTreeData(*item.GetItemData()));

  AddNextComponent(std::make_unique<TreeDataFromTaggedItems>(*item.GetTaggedItems(), item_filter));
}

}  // namespace mvvm
