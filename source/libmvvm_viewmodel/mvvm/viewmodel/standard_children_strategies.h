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

#ifndef MVVM_VIEWMODEL_STANDARDCHILDRENSTRATEGIES_H
#define MVVM_VIEWMODEL_STANDARDCHILDRENSTRATEGIES_H

//! @file childrenstrategies.h
//! @brief Collection of strategies to find children, actual of fictional, of given SessionItem.
//! Used for ViewModel generation when underlying SessionModel changes its layout.

#include "mvvm/interfaces/children_strategy_interface.h"

namespace mvvm
{
class SessionItem;

//! Strategy to find children of given item: gives all actual children back.
//! Hidden children by the current convention will be also in the list.

class MVVM_VIEWMODEL_EXPORT AllChildrenStrategy : public ChildrenStrategyInterface
{
public:
  std::vector<SessionItem*> GetChildren(const SessionItem* item) const override;
};

//! Strategy to find children of given item: only top level items will be given, all
//! property items will be filtered out. All items explicitly marked with setVisible(false)
//! will not show up.

class MVVM_VIEWMODEL_EXPORT TopItemsStrategy : public ChildrenStrategyInterface
{
public:
  std::vector<SessionItem*> GetChildren(const SessionItem* item) const override;
};

//! Strategy to find children of given item: only property item will be given, all top level items
//! will be filtered out, all inactive children of GroupItem will be filtered out. See example
//! in code.

class MVVM_VIEWMODEL_EXPORT PropertyItemsStrategy : public ChildrenStrategyInterface
{
public:
  std::vector<SessionItem*> GetChildren(const SessionItem* item) const override;
};
}  // namespace mvvm

#endif  // MVVM_VIEWMODEL_STANDARDCHILDRENSTRATEGIES_H
