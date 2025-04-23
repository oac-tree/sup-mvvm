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

#ifndef MVVM_VIEWMODEL_STANDARD_CHILDREN_STRATEGIES_H_
#define MVVM_VIEWMODEL_STANDARD_CHILDREN_STRATEGIES_H_

//! @file
//! Collection of strategies to find children, actual of fictional, of given SessionItem. Used for
//! ViewModel generation when underlying SessionModel changes its layout.

#include <mvvm/viewmodel/i_children_strategy.h>

#include <string>

namespace mvvm
{

class SessionItem;

/**
 * @brief The AllChildrenStrategy class finds all actual children of a given item.
 *
 * Hidden children by the current convention will also be on the list.
 */
class MVVM_VIEWMODEL_EXPORT AllChildrenStrategy : public IChildrenStrategy
{
public:
  std::vector<SessionItem*> GetChildren(const SessionItem* item) const override;
};

/**
 * @brief The AllVisibleChildrenStrategy class finds all visible children of a given item.
 */
class MVVM_VIEWMODEL_EXPORT AllVisibleChildrenStrategy : public IChildrenStrategy
{
public:
  std::vector<SessionItem*> GetChildren(const SessionItem* item) const override;
};

/**
 * @brief The TopItemsStrategy class finds children of given items that are top-level items.
 *
 * This will filter out all items marked with a property flag. Also, all items explicitly
 * marked with setVisible(false) will not show up.
 */
class MVVM_VIEWMODEL_EXPORT TopItemsStrategy : public IChildrenStrategy
{
public:
  std::vector<SessionItem*> GetChildren(const SessionItem* item) const override;
};

/**
 * @brief The PropertyItemsStrategy class finds children who are marked as properties.
 *
 * All top-level items will be filtered out.
 */
class MVVM_VIEWMODEL_EXPORT PropertyItemsStrategy : public IChildrenStrategy
{
public:
  std::vector<SessionItem*> GetChildren(const SessionItem* item) const override;
};

/**
 * The FixedItemTypeStrategy class finds children that belong to one of the given types.
 */
class MVVM_VIEWMODEL_EXPORT FixedItemTypeStrategy : public IChildrenStrategy
{
public:
  explicit FixedItemTypeStrategy(std::vector<std::string> item_types);

  std::vector<SessionItem*> GetChildren(const SessionItem* item) const override;

private:
  std::vector<std::string> m_item_types;
};

}  // namespace mvvm

#endif  // MVVM_VIEWMODEL_STANDARD_CHILDREN_STRATEGIES_H_
