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

#ifndef SAVELOADPROJECT_SAVELOADPROJECTCORE_MAIN_WINDOW_H_
#define SAVELOADPROJECT_SAVELOADPROJECTCORE_MAIN_WINDOW_H_

#include <QMainWindow>
#include <memory>

namespace mvvm
{
class AppProject;
class ProjectContext;
class ProjectHandler;
}

namespace saveloadproject
{

class SampleModel;
class ModelEditorWidget;
class MainWindowActions;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow();
  ~MainWindow() override;

protected:
  void closeEvent(QCloseEvent* event) override;

private:
  void InitApplication();
  void WriteSettings();

  /**
   * @brief Perform widgets setup on new project creation or project load from disk.
   */
  void OnProjectLoad();

  /**
   * @brief Update recent project names and the name of the modified project.
   */
  void UpdateProjectNames();

  /**
   * @brief Creates application project.
   */
  std::unique_ptr<mvvm::AppProject> CreateProject();

  ModelEditorWidget* m_editor_widget{nullptr};
  std::unique_ptr<mvvm::AppProject> m_project;
  MainWindowActions* m_actions{nullptr};
};

}  // namespace saveloadproject

#endif  // SAVELOADPROJECT_SAVELOADPROJECTCORE_MAIN_WINDOW_H_
