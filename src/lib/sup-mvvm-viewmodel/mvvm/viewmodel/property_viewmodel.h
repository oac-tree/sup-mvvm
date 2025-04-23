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

#ifndef MVVM_VIEWMODEL_PROPERTY_VIEWMODEL_H_
#define MVVM_VIEWMODEL_PROPERTY_VIEWMODEL_H_

#include <mvvm/viewmodel/viewmodel.h>

namespace mvvm
{

class ISessionModel;

//! View model to show content of SessionModel in Qt widgets.
//! Only property items are shown, also hides inactive items of GroupProperty.

class MVVM_VIEWMODEL_EXPORT PropertyViewModel : public ViewModel
{
  Q_OBJECT

public:
  explicit PropertyViewModel(ISessionModel* model, QObject* parent_object = nullptr);
};

}  // namespace mvvm

#endif  // MVVM_VIEWMODEL_PROPERTY_VIEWMODEL_H_
