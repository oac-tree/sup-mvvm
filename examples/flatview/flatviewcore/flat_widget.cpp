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

#include "flat_widget.h"

#include <mvvm/viewmodel/viewmodel.h>
#include <mvvm/widgets/property_flat_view.h>

#include <QVBoxLayout>

namespace flatview
{

FlatWidget::FlatWidget(QWidget *parent) : QWidget(parent), m_flat_view(new mvvm::PropertyFlatView)
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_flat_view);
}

FlatWidget::~FlatWidget() = default;

void FlatWidget::SetViewModel(std::unique_ptr<mvvm::ViewModel> view_model)
{
  m_view_model = std::move(view_model);
  m_flat_view->SetViewModel(m_view_model.get());
}

void FlatWidget::SetItem(mvvm::SessionItem *item)
{
  m_view_model->SetRootSessionItem(item);
}

}  // namespace flatview
