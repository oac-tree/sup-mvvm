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

#ifndef MVVM_INTERFACES_CELLDECORATORINTERFACE_H
#define MVVM_INTERFACES_CELLDECORATORINTERFACE_H

#include "mvvm/viewmodel_export.h"

#include <string>

class QModelIndex;
class QStyleOptionViewItem;

namespace mvvm
{

//! Interface class to generate cell decorations (i.e. text) in Qt trees and tables.

class MVVM_VIEWMODEL_EXPORT CellDecoratorInterface
{
public:
  virtual ~CellDecoratorInterface() = default;

  virtual bool HasCustomDecoration(const QModelIndex& index) const = 0;
  virtual void InitStyleOption(QStyleOptionViewItem* option, const QModelIndex& index) = 0;
};

}  // namespace mvvm

#endif  // MVVM_INTERFACES_CELLDECORATORINTERFACE_H
