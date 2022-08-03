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

#include "container_editor_widget.h"
#include "sample_model.h"

#include <mvvm/commands/command_stack_interface.h>
#include <mvvm/model/model_utils.h>

#include <QAction>
#include <QHBoxLayout>
#include <QToolBar>
#include <QToolButton>

namespace DragAndMove
{

ModelEditorWidget::ModelEditorWidget(SampleModel* model, QWidget* parent)
    : QWidget(parent)
    , m_tool_bar(new QToolBar)
    , m_left_widget(new ContainerEditorWidget)
    , m_right_widget(new ContainerEditorWidget)
{
  auto main_layout = new QVBoxLayout(this);
  main_layout->setSpacing(5);

  auto top_layout = new QVBoxLayout;
  top_layout->addWidget(m_tool_bar);

  auto container_layout = new QHBoxLayout;
  container_layout->setSpacing(10);
  container_layout->addWidget(m_left_widget);
  container_layout->addSpacing(20);
  container_layout->addWidget(m_right_widget);

  main_layout->addLayout(top_layout);
  main_layout->addLayout(container_layout);

  SetModel(model);

  SetupActions();
}

void ModelEditorWidget::SetModel(SampleModel* model)
{
  if (!model)
  {
    return;
  }

  m_model = model;

  auto containers = mvvm::utils::GetTopItems(m_model);

  m_left_widget->SetModel(m_model, containers[0]);
  m_right_widget->SetModel(m_model, containers[1]);
}

void ModelEditorWidget::OnUndo()
{
  mvvm::utils::Undo(*m_model);
  UpdateActionAvailability();
}

void ModelEditorWidget::OnRedo()
{
  mvvm::utils::Redo(*m_model);
  UpdateActionAvailability();
}

void ModelEditorWidget::SetupActions()
{
  const int toolbar_icon_size = 24;
  m_tool_bar->setIconSize(QSize(toolbar_icon_size, toolbar_icon_size));

  m_undo_action = new QAction("Undo", this);
  connect(m_undo_action, &QAction::triggered, this, &ModelEditorWidget::OnUndo);
  m_tool_bar->addAction(m_undo_action);

  m_redo_action = new QAction("Redo", this);
  connect(m_redo_action, &QAction::triggered, this, &ModelEditorWidget::OnRedo);
  m_tool_bar->addAction(m_redo_action);
}

void ModelEditorWidget::UpdateActionAvailability()
{
//    m_redo_action->setEnabled(m_model->GetCommandStack()->CanRedo());
//    m_undo_action->setEnabled(m_model->GetCommandStack()->CanUndo());
}

}  // namespace DragAndMove
