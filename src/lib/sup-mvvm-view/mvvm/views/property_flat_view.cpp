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

#include "property_flat_view.h"

#include <mvvm/views/property_grid_controller.h>
#include <mvvm/widgets/layout_utils.h>

#include <QAbstractItemModel>
#include <QGridLayout>
#include <QVBoxLayout>

namespace mvvm
{

PropertyFlatView::PropertyFlatView(QWidget *parent_widget)
    : QWidget(parent_widget), m_grid_layout(new QGridLayout)
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  m_grid_layout->setSpacing(6);
  layout->addLayout(m_grid_layout);
  layout->addStretch(1);
}

void PropertyFlatView::SetViewModel(QAbstractItemModel *view_model)
{
  m_view_model = view_model;

  m_grid_controller = std::make_unique<PropertyGridController>(m_view_model);

  connect(m_grid_controller.get(), &PropertyGridController::GridChanged, this,
          &PropertyFlatView::UpdateGridLayout);

  UpdateGridLayout();
}

void PropertyFlatView::UpdateGridLayout()
{
  ClearGridLayout(m_grid_layout, true);

  auto widgets = m_grid_controller->CreateWidgetGrid();

  for (int row = 0; row < m_view_model->rowCount(); ++row)
  {
    for (int col = 0; col < m_view_model->columnCount(); ++col)
    {
      m_grid_layout->addWidget(widgets[row][col].release(), row, col);
    }
  }
}

PropertyFlatView::~PropertyFlatView() = default;

}  // namespace mvvm
