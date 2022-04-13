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

#include "mvvm/widgets/abstractitemview.h"

#include "mvvm/core/exceptions.h"
#include "mvvm/model/applicationmodel.h"
#include "mvvm/viewmodel/viewmodel.h"

namespace mvvm
{

AbstractItemView::AbstractItemView(QWidget *parent) : ItemViewBase(parent) {}

void AbstractItemView::SetApplicationModel(ApplicationModel *model)
{
  if (!model)
  {
    Reset();
    return;
  }
  SetViewModel(CreateViewModel(model));
}

void AbstractItemView::SetItem(SessionItem *item)
{
  if (!GetViewModel())
  {
    auto application_model = dynamic_cast<ApplicationModel *>(item->GetModel());
    if (!application_model)
    {
      throw RuntimeException("Error in PropertyTreeView: wrong type model");
    }
    SetApplicationModel(application_model);
  }
  SetRootSessionItem(item);
}

}  // namespace mvvm
