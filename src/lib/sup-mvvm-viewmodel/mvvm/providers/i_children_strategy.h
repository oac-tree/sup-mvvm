/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#ifndef MVVM_PROVIDERS_I_CHILDREN_STRATEGY_H_
#define MVVM_PROVIDERS_I_CHILDREN_STRATEGY_H_

#include <vector>

namespace mvvm
{

class SessionItem;

/**
 * @brief The IChildrenStrategy class is an interface for all strategies to find children,
 * actual or fictional, of a given item.
 *
 * The reported vector of children might be different from the actual children of a given item.
 */
class IChildrenStrategy
{
public:
  IChildrenStrategy() = default;
  virtual ~IChildrenStrategy() = default;

  IChildrenStrategy(const IChildrenStrategy&) = delete;
  IChildrenStrategy& operator=(const IChildrenStrategy&) = delete;
  IChildrenStrategy(IChildrenStrategy&&) = delete;
  IChildrenStrategy& operator=(IChildrenStrategy&&) = delete;

  /**
   * @brief Returns vector of children of given item.
   *
   * The strategy is used in the context of AbstractViewModel while exposing SessionModel to Qt.
   * Thanks to this strategy ViewModel decides which items to visit.
   *
   * The item is non-const because the reported children are the items the view models build
   * editable views for; mutable children cannot honestly be derived from a const parent.
   */
  virtual std::vector<SessionItem*> GetChildren(SessionItem* item) const = 0;
};

}  // namespace mvvm

#endif  // MVVM_PROVIDERS_I_CHILDREN_STRATEGY_H_
