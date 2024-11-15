/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
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
#include "recent_project_widget.h"
#include "sample_model.h"

#include <mvvm/commands/i_command_stack.h>
#include <mvvm/model/model_utils.h>
#include <mvvm/widgets/widget_utils.h>

#include <QAction>
#include <QHBoxLayout>
#include <QToolBar>
#include <QToolButton>

namespace saveloadproject
{

ModelEditorWidget::ModelEditorWidget(QWidget* parent)
    : QWidget(parent)
    , m_recent_project_widget(new RecentProjectWidget)
    , m_right_widget(new ContainerEditorWidget)
{
  auto main_layout = new QVBoxLayout(this);
  main_layout->setSpacing(5);

  auto top_layout = new QVBoxLayout;

  auto container_layout = new QHBoxLayout;
  container_layout->setSpacing(10);
  container_layout->addWidget(m_recent_project_widget);
  container_layout->addSpacing(20);
  container_layout->addWidget(m_right_widget);

  main_layout->addLayout(top_layout);
  main_layout->addLayout(container_layout);
}

ModelEditorWidget::~ModelEditorWidget() = default;

void ModelEditorWidget::SetModel(SampleModel* model)
{
  if (!model)
  {
    return;
  }

  m_model = model;

  auto containers = mvvm::utils::GetTopItems(m_model);

  m_right_widget->SetModel(m_model, containers[1]);
}

void ModelEditorWidget::UpdateCurrentProjectInfo(const std::string& path, bool is_modified)
{
  m_recent_project_widget->UpdateCurrentProjectInfo(QString::fromStdString(path), is_modified);
}

void ModelEditorWidget::UpdateRecentProjectList(const std::vector<std::string>& recent_list)
{
  m_recent_project_widget->UpdateRecentProjectsList(mvvm::utils::GetStringList(recent_list));
}

}  // namespace saveloadproject
