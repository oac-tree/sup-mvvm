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

#include "main_window.h"

#include "main_window_actions.h"
#include "model_editor_widget.h"
#include "sample_model.h"

#include <mvvm/project/app_project.h>
#include <mvvm/standarditems/container_item.h>

#include <QCoreApplication>
#include <QSettings>

namespace
{
const QString main_window_group = "MainWindow";
const QString size_key = "size";
const QString pos_key = "pos";
}  // namespace

namespace saveloadproject
{

MainWindow::MainWindow()
    : m_project(CreateProject()), m_actions(new MainWindowActions(m_project.get(), this))
{
  InitApplication();

  m_project->CreateEmpty();
}

MainWindow::~MainWindow() = default;

void MainWindow::closeEvent(QCloseEvent* event)
{
  if (m_actions->CloseCurrentProject())
  {
    WriteSettings();
    QMainWindow::closeEvent(event);
  }
}

void MainWindow::InitApplication()
{
  QSettings settings;
  if (settings.childGroups().contains(main_window_group))
  {
    settings.beginGroup(main_window_group);
    resize(settings.value(size_key, QSize(400, 400)).toSize());
    move(settings.value(pos_key, QPoint(200, 200)).toPoint());
    settings.endGroup();
  }

  m_editor_widget = new ModelEditorWidget;
  connect(m_editor_widget, &ModelEditorWidget::projectSelected, m_actions,
          &MainWindowActions::OpenExistingProject);

  setCentralWidget(m_editor_widget);
}

void MainWindow::WriteSettings()
{
  QSettings settings;
  settings.beginGroup(main_window_group);
  settings.setValue(size_key, size());
  settings.setValue(pos_key, pos());
  settings.endGroup();
}

void MainWindow::OnProjectLoad()
{
  m_editor_widget->SetModel(m_project->GetModel<SampleModel>());
  UpdateProjectNames();
}

void MainWindow::UpdateProjectNames()
{
  m_actions->UpdateNames();
  m_editor_widget->UpdateCurrentProjectInfo(m_project->GetPath(), m_project->IsModified());
  m_editor_widget->UpdateRecentProjectList(m_actions->GetRecentProjectList());
}

std::unique_ptr<mvvm::AppProject> MainWindow::CreateProject()
{
  mvvm::ProjectContext context;
  context.modified_callback = [this]() { UpdateProjectNames(); };
  context.loaded_callback = [this]() { OnProjectLoad(); };
  context.saved_callback = [this]() { UpdateProjectNames(); };

  auto result = std::make_unique<mvvm::AppProject>(context);
  result->RegisterModel<SampleModel>();
  return result;
}

}  // namespace saveloadproject
