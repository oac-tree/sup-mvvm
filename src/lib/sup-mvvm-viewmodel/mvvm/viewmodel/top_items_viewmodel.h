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

#ifndef MVVM_VIEWMODEL_TOP_ITEMS_VIEWMODEL_H_
#define MVVM_VIEWMODEL_TOP_ITEMS_VIEWMODEL_H_

#include <mvvm/viewmodel/viewmodel.h>

namespace mvvm
{

class ISessionModel;

//! View model to show top level items of SessionModel in Qt trees and tables.
//! All property items (i.e. "thickness", "color" etc) will be filtered out, top level items
//! (i.e. Layer, MultiLayer, ...) will be presented as simple parent/child tree.

class MVVM_VIEWMODEL_EXPORT TopItemsViewModel : public ViewModel
{
  Q_OBJECT

public:
  explicit TopItemsViewModel(ISessionModel* model, QObject* parent = nullptr);
};

}  // namespace mvvm

#endif  // MVVM_VIEWMODEL_TOP_ITEMS_VIEWMODEL_H_
