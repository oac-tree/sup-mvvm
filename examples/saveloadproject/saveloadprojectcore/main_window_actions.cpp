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

#include "main_window_actions.h"

#include <mvvm/project/project_handler.h>
#include <mvvm/project/project_handler_utils.h>

#include <QAction>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>

namespace saveloadproject
{

MainWindowActions::MainWindowActions(mvvm::IProject *project, QMainWindow *main_window)
    : QObject(main_window)
    , m_exit_action(new QAction("E&xit Application", this))
    , m_recent_project_menu(new QMenu())
    , m_project_handler(std::make_unique<mvvm::ProjectHandler>(project))
{
  SetupMenu(main_window);
}

bool MainWindowActions::CloseCurrentProject() const
{
  return m_project_handler->CloseProject();
}

void MainWindowActions::UpdateNames()
{
  m_project_handler->UpdateNames();
}

std::vector<std::string> MainWindowActions::GetRecentProjectList() const
{
  return m_project_handler->GetRecentProjectList();
}

void MainWindowActions::OpenExistingProject(const QString &path)
{
  m_project_handler->OpenExistingProject(path.toStdString());
}

MainWindowActions::~MainWindowActions() = default;

void MainWindowActions::SetupMenu(QMainWindow *main_window)
{
  auto file_menu = main_window->menuBar()->addMenu("&File");

  AddNewProjectAction(file_menu, *m_project_handler);
  AddOpenExistingProjectAction(file_menu, *m_project_handler);

  m_recent_project_menu = file_menu->addMenu("Recent Projects");
  auto about_to_show_menu = [this]()
  { AddRecentProjectActions(m_recent_project_menu, *m_project_handler); };
  connect(file_menu, &QMenu::aboutToShow, this, about_to_show_menu);

  file_menu->addSeparator();
  AddSaveCurrentProjectAction(file_menu, *m_project_handler);
  AddSaveProjectAsAction(file_menu, *m_project_handler);

  file_menu->addSeparator();

  m_exit_action->setShortcuts(QKeySequence::Quit);
  m_exit_action->setStatusTip("Exit the application");
  connect(m_exit_action, &QAction::triggered, main_window, &QMainWindow::close);
  file_menu->addAction(m_exit_action);
}

}  // namespace saveloadproject
