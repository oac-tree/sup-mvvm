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

#include "mvvm/viewmodel/topitemsviewmodel.h"

#include "mvvm/viewmodel/applicationmodel.h"
#include "mvvm/viewmodel/standardchildrenstrategies.h"
#include "mvvm/viewmodel/standardrowstrategies.h"
#include "mvvm/viewmodel/viewmodelcontroller.h"

namespace mvvm
{
TopItemsViewModel::TopItemsViewModel(ApplicationModel *model, QObject *parent) : ViewModel(parent)
{
  auto controller = std::make_unique<ViewModelController>(model, this);
  model->Subscribe(controller.get());
  controller->SetChildrenStrategy(std::make_unique<TopItemsStrategy>());

  auto set_data = [model](auto item, auto data, auto role)
  { return model->SetData(item, data, role); };

  controller->SetRowStrategy(std::make_unique<LabelDataRowStrategy>(set_data));
  SetController(std::move(controller));
}

}  // namespace ModelView
