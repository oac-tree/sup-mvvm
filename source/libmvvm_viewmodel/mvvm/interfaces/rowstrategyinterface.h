/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_INTERFACES_ROWSTRATEGYINTERFACE_H
#define MVVM_INTERFACES_ROWSTRATEGYINTERFACE_H

#include "mvvm/viewmodel_export.h"

#include <QStringList>
#include <memory>
#include <vector>

namespace mvvm
{
class SessionItem;
class ViewItem;

//! Base class to construct row of ViewItem's from given SessionItem.
//! Used in context of AbstractViewModel while exposing SessionModel to Qt.

class MVVM_VIEWMODEL_EXPORT RowStrategyInterface
{
public:
  virtual ~RowStrategyInterface() = default;

  virtual QStringList GetHorizontalHeaderLabels() const = 0;

  virtual std::vector<std::unique_ptr<ViewItem>> ConstructRow(SessionItem*) = 0;
};

}  // namespace mvvm

#endif  // MVVM_INTERFACES_ROWSTRATEGYINTERFACE_H
