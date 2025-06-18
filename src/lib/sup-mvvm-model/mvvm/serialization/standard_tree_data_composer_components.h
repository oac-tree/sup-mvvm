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

#ifndef MVVM_SERIALIZATION_STANADRD_TREE_DATA_COMPOSER_COMPONENTS_H_
#define MVVM_SERIALIZATION_STANADRD_TREE_DATA_COMPOSER_COMPONENTS_H_

//! @file
//! Collection of TreeDataComposerComponent classes to build TreeData from SessionItem.

#include <mvvm/serialization/converter_types.h>
#include <mvvm/serialization/tree_data_composer_component.h>

namespace mvvm
{

class SessionItem;
class SessionItemContainer;
class TaggedItems;

/**
 * @brief The TreeDataFromItemContainer class assists in building TreeData From
 * SessionItemContainer.
 */
class TreeDataFromItemContainer : public TreeDataComposerComponent
{
public:
  explicit TreeDataFromItemContainer(const mvvm::SessionItemContainer& container,
                                     const item_filter_t& item_filter);
};

/**
 * @brief The TreeDataFromTaggedItems class assists in building TreeData from TaggedItems.
 */
class TreeDataFromTaggedItems : public TreeDataComposerComponent
{
public:
  explicit TreeDataFromTaggedItems(const mvvm::TaggedItems& tagged_items,
                                   const item_filter_t& item_filter);
};

/**
 * @brief The TreeDataFromSessionItem class assists in building TreeData from SessionItem.
 */
class TreeDataFromSessionItem : public TreeDataComposerComponent
{
public:
  explicit TreeDataFromSessionItem(const mvvm::SessionItem& item, const item_filter_t& item_filter);
};

}  // namespace mvvm

#endif  // MVVM_SERIALIZATION_STANADRD_TREE_DATA_COMPOSER_COMPONENTS_H_
