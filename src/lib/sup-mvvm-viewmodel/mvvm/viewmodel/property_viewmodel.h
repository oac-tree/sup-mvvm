/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_VIEWMODEL_PROPERTY_VIEWMODEL_H_
#define MVVM_VIEWMODEL_PROPERTY_VIEWMODEL_H_

#include <mvvm/viewmodel/viewmodel.h>

namespace mvvm
{

class SessionModelInterface;

//! View model to show content of SessionModel in Qt widgets.
//! Only property items are shown, also hides inactive items of GroupProperty.

class MVVM_VIEWMODEL_EXPORT PropertyViewModel : public ViewModel
{
  Q_OBJECT

public:
  explicit PropertyViewModel(SessionModelInterface* model, QObject* parent = nullptr);

  int columnCount(const QModelIndex& parent = QModelIndex()) const override;
};

}  // namespace mvvm

#endif  // MVVM_VIEWMODEL_PROPERTY_VIEWMODEL_H_
