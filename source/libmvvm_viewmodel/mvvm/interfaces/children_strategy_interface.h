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

#ifndef MVVM_INTERFACES_CHILDRENSTRATEGYINTERFACE_H
#define MVVM_INTERFACES_CHILDRENSTRATEGYINTERFACE_H

#include <mvvm/viewmodel_export.h>

#include <vector>

namespace mvvm
{
class SessionItem;

//! Base class for strategies to find children, actual or fictional, of a given item.
//! The reported vector of children might be different from the actual children of a given item.
//! The strategy is used in the context of AbstractViewModel while exposing SessionModel to Qt.
//! Thanks to this strategy ViewModel decides which items to visit.

class MVVM_VIEWMODEL_EXPORT ChildrenStrategyInterface
{
public:
  virtual ~ChildrenStrategyInterface() = default;

  //! Returns vector of children of given item.
  virtual std::vector<SessionItem*> GetChildren(const SessionItem* item) const = 0;
};

}  // namespace mvvm

#endif  // MVVM_INTERFACES_CHILDRENSTRATEGYINTERFACE_H
