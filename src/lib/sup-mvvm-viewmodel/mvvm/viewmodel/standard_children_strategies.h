/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_VIEWMODEL_STANDARD_CHILDREN_STRATEGIES_H_
#define MVVM_VIEWMODEL_STANDARD_CHILDREN_STRATEGIES_H_

//! @file
//! Collection of strategies to find children, actual of fictional, of given SessionItem. Used for
//! ViewModel generation when underlying SessionModel changes its layout.

#include <mvvm/interfaces/children_strategy_interface.h>

namespace mvvm
{

class SessionItem;

/**
 * @brief The AllChildrenStrategy class finds all actual children of given item.
 *
 * @details Hidden children by the current convention will be also in the list.
 */
class MVVM_VIEWMODEL_EXPORT AllChildrenStrategy : public ChildrenStrategyInterface
{
public:
  std::vector<SessionItem*> GetChildren(const SessionItem* item) const override;
};

/**
 * @brief The TopItemsStrategy class finds children of given item which are top-level items.
 *
 * @details This will filter out all items marked with property flag. Also, all items explicitly
 * marked with setVisible(false) will not show up.
 */
class MVVM_VIEWMODEL_EXPORT TopItemsStrategy : public ChildrenStrategyInterface
{
public:
  std::vector<SessionItem*> GetChildren(const SessionItem* item) const override;
};

/**
 * @brief The PropertyItemsStrategy class finds children which are marked as properties.
 *
 * @details All top level items will be filtered out.
 */
class MVVM_VIEWMODEL_EXPORT PropertyItemsStrategy : public ChildrenStrategyInterface
{
public:
  std::vector<SessionItem*> GetChildren(const SessionItem* item) const override;
};
}  // namespace mvvm

#endif  // MVVM_VIEWMODEL_STANDARD_CHILDREN_STRATEGIES_H_
