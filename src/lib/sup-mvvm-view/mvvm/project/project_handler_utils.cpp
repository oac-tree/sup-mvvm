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

#include "project_handler_utils.h"

#include <mvvm/project/i_project_manager.h>
#include <mvvm/widgets/widget_utils.h>

#include <QAction>
#include <QMenu>

namespace mvvm
{

namespace
{

// empty name will trigger file selection dialog
const std::string kAskForName = "";

/**
 * @brief Creates an action to trigger "create new project" request.
 */
std::unique_ptr<QAction> CreateNewProjectAction(IProjectManager &handler)
{
  auto result = std::make_unique<QAction>("&New Project");
  result->setShortcuts(QKeySequence::New);
  result->setStatusTip("Create a new project");
  QObject::connect(result.get(), &QAction::triggered,
                   [&handler]() { handler.CreateNewProject(kAskForName); });
  return result;
}

/**
 * @brief Creates an action to trigger "open existing project" request.
 */
std::unique_ptr<QAction> CreateOpenExistingProjectAction(IProjectManager &handler)
{
  auto result = std::make_unique<QAction>("&Open Project");
  result->setShortcuts(QKeySequence::Open);
  result->setStatusTip("Open an existing project");
  QObject::connect(result.get(), &QAction::triggered,
                   [&handler]() { handler.OpenExistingProject(kAskForName); });
  return result;
}

/**
 * @brief Creates an action to trigger "save current project" request.
 */
std::unique_ptr<QAction> CreateSaveCurrentProjectAction(IProjectManager &handler)
{
  auto result = std::make_unique<QAction>("&Save Project");
  result->setShortcuts(QKeySequence::Save);
  result->setStatusTip("Save project");
  result->setShortcutContext(Qt::ApplicationShortcut);
  QObject::connect(result.get(), &QAction::triggered,
                   [&handler]() { handler.SaveCurrentProject(); });
  return result;
}

/**
 * @brief Creates an action to trigger "save project as" request.
 */
std::unique_ptr<QAction> CreateSaveProjectAsAction(IProjectManager &handler)
{
  auto result = std::make_unique<QAction>("Save Project &As...");
  result->setShortcuts(QKeySequence::SaveAs);
  result->setStatusTip("Save project under different name");
  QObject::connect(result.get(), &QAction::triggered,
                   [&handler]() { handler.SaveProjectAs(kAskForName); });
  return result;
}

/**
 * @brief Adds action to the menu, transfer ownership.
 */
void AddAction(QMenu *menu, std::unique_ptr<QAction> action)
{
  action->setParent(menu);  // transfer ownerhip
  menu->addAction(action.release());
}

}  // namespace

void AddNewProjectAction(QMenu *menu, IProjectManager &handler)
{
  AddAction(menu, CreateNewProjectAction(handler));
}

void AddOpenExistingProjectAction(QMenu *menu, IProjectManager &handler)
{
  AddAction(menu, CreateOpenExistingProjectAction(handler));
}

void AddSaveCurrentProjectAction(QMenu *menu, IProjectManager &handler)
{
  AddAction(menu, CreateSaveCurrentProjectAction(handler));
}

void AddSaveProjectAsAction(QMenu *menu, IProjectManager &handler)
{
  AddAction(menu, CreateSaveProjectAsAction(handler));
}

void AddRecentProjectActions(QMenu *menu, IProjectManager &handler)
{
  auto recent_projects = handler.GetRecentProjectList();
  menu->clear();
  menu->setEnabled(!recent_projects.empty());

  for (const auto &path : recent_projects)
  {
    auto trimmed_path = utils::WithTildeHomePath(QString::fromStdString(path));
    auto action = menu->addAction(trimmed_path);
    action->setData(QVariant::fromValue(QString::fromStdString(path)));
    auto on_project_selected = [&handler, path]() { handler.OpenExistingProject(path); };
    QObject::connect(action, &QAction::triggered, on_project_selected);
  }

  if (!recent_projects.empty())
  {
    menu->addSeparator();
    auto action = menu->addAction("Clear Menu");
    QAction::connect(action, &QAction::triggered,
                     [&handler]() { handler.ClearRecentProjectsList(); });
  }
}

}  // namespace mvvm
