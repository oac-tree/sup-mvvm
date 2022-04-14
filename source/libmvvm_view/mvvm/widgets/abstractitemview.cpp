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

AbstractItemView::AbstractItemView(create_viewmodel_t func, QAbstractItemView *view,
                                   ApplicationModel *model, QWidget *parent)
    : ItemViewBase(parent), m_create_viewmodel(std::move(func))
{
  SetView(view);
  SetApplicationModel(model);
}

AbstractItemView::~AbstractItemView() = default;

void AbstractItemView::SetApplicationModel(ApplicationModel *model)
{
  if (!model)
  {
    Reset();
    return;
  }
  SetViewModel(m_create_viewmodel(model));
}

void AbstractItemView::SetItem(SessionItem *item)
{
  if (!item)
  {
    Reset();
    return;
  }

  // FIXME how to handle situation when item belongs to another model?

  if (!GetViewModel())
  {
    auto application_model = dynamic_cast<ApplicationModel *>(item->GetModel());
    if (!application_model)
    {
      throw RuntimeException("Wrong model type");
    }
    SetApplicationModel(application_model);
  }
  SetRootSessionItem(item);
}

}  // namespace mvvm
