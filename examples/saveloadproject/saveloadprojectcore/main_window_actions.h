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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SAVELOADPROJECT_SAVELOADPROJECTCORE_MAIN_WINDOW_ACTIONS_H_
#define SAVELOADPROJECT_SAVELOADPROJECTCORE_MAIN_WINDOW_ACTIONS_H_

#include <QObject>
#include <memory>

class QMainWindow;
class QMenu;
class QAction;
class QMenuBar;

namespace mvvm
{
class IProject;
class ProjectHandler;
}  // namespace mvvm

namespace saveloadproject
{

/**
 * @brief The MainWindowActions class is responsible to setup file menu and handle
 * save/save-as/load/exit activities.
 */
class MainWindowActions : public QObject
{
  Q_OBJECT

public:
  explicit MainWindowActions(mvvm::IProject* project, QMainWindow* main_window);
  ~MainWindowActions() override;

  /**
   * @brief Closes current project.
   *
   * Internally performs check for unsaved data, and proceeds via save/discard/cancel dialog.
   * Returns true if project was successfully saved or user has agreed to discard unsaved changes,
   * and false otherwise. The later normally means that the user has changed his mind in the course
   * of this operation, canceled dialog, and the project has remained in unsaved state.
   *
   * @return True in the case of success.
   */
  bool CloseCurrentProject() const;

  void UpdateNames();

  std::vector<std::string> GetRecentProjectList() const;

  void OpenExistingProject(const QString& path);

private:
  void SetupMenu(QMainWindow* main_window);

  QAction* m_exit_action{nullptr};
  QMenu* m_recent_project_menu{nullptr};
  std::unique_ptr<mvvm::ProjectHandler> m_project_handler;
};

}  // namespace saveloadproject

#endif  // SAVELOADPROJECT_SAVELOADPROJECTCORE_MAIN_WINDOW_ACTIONS_H_
