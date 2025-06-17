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

#include <mvvm/serialization/tree_data_composer_component.h>

namespace mvvm
{

class SessionItem;
class SessionItemContainer;
class TaggedItems;

/**
 * @brief The ItemContainerComposerComponent class assists in building TreeData From
 * SessionItemContainer.
 */
class ItemContainerComposerComponent : public TreeDataComposerComponent
{
public:
  explicit ItemContainerComposerComponent(const mvvm::SessionItemContainer& container);
};

/**
 * @brief The TaggedItemsComposerComponent class assists in building TreeData from TaggedItems.
 */
class TaggedItemsComposerComponent : public TreeDataComposerComponent
{
public:
  explicit TaggedItemsComposerComponent(const mvvm::TaggedItems& tagged_items);
};

/**
 * @brief The ItemComposerComponent class assists in building TreeData from SessionItem.
 */
class ItemComposerComponent : public TreeDataComposerComponent
{
public:
  explicit ItemComposerComponent(const mvvm::SessionItem& item);
};

}  // namespace mvvm

#endif  // MVVM_SERIALIZATION_STANADRD_TREE_DATA_COMPOSER_COMPONENTS_H_
