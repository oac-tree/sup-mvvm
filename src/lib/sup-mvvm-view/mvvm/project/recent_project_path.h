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

#ifndef MVVM_PROJECT_RECENT_PROJECT_PATH_H_
#define MVVM_PROJECT_RECENT_PROJECT_PATH_H_

#include <QStringList>

namespace mvvm
{

/**
 * @brief The RecentProjectPath class is intended to store the last path selected by the user and a
 * list of recent projects.
 *
 * The paths can be either the path to existing files or path to the directory. The class is
 * also responsible for validating the presence of files on disk.
 */
class RecentProjectPath
{
public:
  /**
   * @brief Main c-tor.
   *
   * @param max_project_count The maximum number of project paths to store.
   */
  explicit RecentProjectPath(int max_project_count);
  virtual ~RecentProjectPath() = default;

  /**
   * @brief Returns current working directory.
   *
   * Working directory is the parent directory of last opened project.
   */
  QString GetCurrentWorkdir() const;

  /**
   * @brief Sets the current working directory to a given value (full path).
   */
  void SetCurrentWorkdir(const QString& dir_name);

  /**
   * @brief Updates the current working directory from the project path.
   *
   * If the project path is a path to a file, then the working directory will be the directory where
   * the file is located. If the project path is a path to a folder, then, similarly, the working
   * directory is the parent dir.
   *
   * @param project_path The path to the project where we save results (full path).
   */
  void UpdateWorkdirFromPath(const QString& project_path);

  /**
   * @brief Returns list of recent projects.
   *
   * The method also validates that projects still exist on disk and vlears the list if necessary
   * (so it is non-const).
   */
  QStringList GetRecentProjectList();

  /**
   * @brief Sets the recent project list to the current value.
   */
  void SetRecentProjectList(const QStringList& recent_project_list);

  /**
   * @brief Adds given project path to the list of recent projects.
   */
  void AddToRecentProjectList(const QString& project_path);

  /**
   * @brief Clears the list of recent projects.
   */
  void ClearRecentProjectsList();

protected:
  /**
   * @brief Validates if projects exist, and update the list to show only existing projects.
   */
  void ValidateIfProjectsExist();

private:
  QString m_current_workdir;
  QStringList m_recent_projects;
  int m_max_recent_project_count;
};

}  // namespace mvvm

#endif  // MVVM_PROJECT_RECENT_PROJECT_PATH_H_
