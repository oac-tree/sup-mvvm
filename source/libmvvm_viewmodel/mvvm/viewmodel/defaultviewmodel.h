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

#ifndef MVVM_VIEWMODEL_DEFAULTVIEWMODEL_H
#define MVVM_VIEWMODEL_DEFAULTVIEWMODEL_H

#include "mvvm/viewmodel/viewmodel.h"

namespace ModelView
{
class ModelComposer;

//! Default view model to show the content of SessionModel in Qt widgets. Represents two-column tree
//! with label/data, with one-to-one child/parent correspondence as in the original SessionModel.
//! All items (including hidden) are shown.

class MVVM_VIEWMODEL_EXPORT DefaultViewModel : public ViewModel
{
  Q_OBJECT

public:
  DefaultViewModel(SessionModel* model, QObject* parent = nullptr);
};

}  // namespace ModelView

#endif  // MVVM_VIEWMODEL_DEFAULTVIEWMODEL_H
