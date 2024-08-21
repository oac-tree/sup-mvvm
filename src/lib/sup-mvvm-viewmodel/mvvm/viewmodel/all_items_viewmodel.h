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

#ifndef MVVM_VIEWMODEL_ALL_ITEMS_VIEWMODEL_H_
#define MVVM_VIEWMODEL_ALL_ITEMS_VIEWMODEL_H_

#include <mvvm/viewmodel/viewmodel.h>

namespace mvvm
{

class ISessionModel;

//! View model to show the whole content of SessionModel in Qt widgets. Represents two-column tree
//! with label/data, with one-to-one child/parent correspondence as in the original SessionModel.
//! All items (including hidden) are shown.

class MVVM_VIEWMODEL_EXPORT AllItemsViewModel : public ViewModel
{
  Q_OBJECT

public:
  explicit AllItemsViewModel(ISessionModel* model, QObject* parent = nullptr);
};

}  // namespace mvvm

#endif  // MVVM_VIEWMODEL_ALL_ITEMS_VIEWMODEL_H_
