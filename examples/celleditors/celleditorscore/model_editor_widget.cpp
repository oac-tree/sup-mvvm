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

#include "model_editor_widget.h"

#include "property_panel.h"
#include "sample_model.h"

#include <mvvm/model/model_utils.h>
#include <mvvm/standarditems/container_item.h>

#include <QBoxLayout>
#include <QTabWidget>

namespace celleditors
{

ModelEditorWidget::ModelEditorWidget(SampleModel* model, QWidget* parent)
    : QWidget(parent)
    , m_tab_widget(new QTabWidget)
    , m_panel0(new PropertyPanel)
    , m_panel1(new PropertyPanel)
{
  auto layout = new QHBoxLayout(this);

  m_tab_widget->addTab(m_panel0, "Basic properties");
  m_tab_widget->addTab(m_panel1, "All integers");

  layout->addWidget(m_tab_widget);

  SetModel(model);
}

void ModelEditorWidget::SetModel(SampleModel* model)
{
  if (!model)
  {
    return;
  }

  auto containers = mvvm::utils::GetTopItems<mvvm::ContainerItem>(model);
  if (containers.size() != 2)
  {
    return;
  }

  m_panel0->SetItem(containers.at(1));
//  m_panel1->SetItem(containers.at(1));
}
}  // namespace celleditors
