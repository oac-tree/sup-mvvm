/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "model_editor_widget.h"

#include "property_panel.h"
#include "sample_model.h"

#include <mvvm/standarditems/container_item.h>

#include <QBoxLayout>
#include <QTabWidget>

namespace celleditors
{

ModelEditorWidget::ModelEditorWidget(SampleModel* model, QWidget* parent)
    : QWidget(parent), m_tab_widget(new QTabWidget)
{
  auto layout = new QHBoxLayout(this);

  layout->addWidget(m_tab_widget);

  SetModel(model);
}

void ModelEditorWidget::SetModel(SampleModel* model)
{
  if (model == nullptr)
  {
    return;
  }

  for (std::uint32_t index = 0; index < model->GetContainerCount(); ++index)
  {
    auto container = model->GetContainer(index);
    auto panel_name = container->GetDisplayName();
    auto panel = std::make_unique<PropertyPanel>();
    panel->SetItem(container);
    m_tab_widget->addTab(panel.release(), QString::fromStdString(panel_name));
  }
}

}  // namespace celleditors
