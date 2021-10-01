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

#include "mvvm/viewmodel/defaultviewmodel.h"

#include "mvvm/model/modelcomposer.h"
#include "mvvm/viewmodel/viewmodelcontroller.h"

namespace ModelView
{
DefaultViewModel::DefaultViewModel(SessionModel *model, QObject *parent) : ViewModel(parent)
{
  auto controller = std::make_unique<ViewModelController>(model, this);
  SetController(std::move(controller));
}

DefaultViewModel::DefaultViewModel(SessionModel *model, ModelComposer *composer, QObject *parent)
    : ViewModel(parent)
{
  auto controller = std::make_unique<ViewModelController>(model, this);
  composer->EstablishConnections(controller.get());
  SetController(std::move(controller));
}

}  // namespace ModelView
