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

#ifndef MVVM_PROJECT_PROJECT_HANDLER_H_
#define MVVM_PROJECT_PROJECT_HANDLER_H_

#include <mvvm/project/i_project_manager.h>

#include <memory>

namespace mvvm
{

class AbstractProjectUserInteractor;
class IProject;
class IProjectManager;
class RecentProjectSettings;

class ProjectHandler : public IProjectManager
{
public:
  explicit ProjectHandler(IProject* project);

  ~ProjectHandler() override;

  bool CreateNewProject(const std::string& path) override;

  bool CloseProject() override;

  bool SaveCurrentProject() override;

  bool SaveProjectAs(const std::string& path) override;

  bool OpenExistingProject(const std::string& path) override;

  IProject* GetProject() const override;

  void ClearRecentProjectsList() override;

  std::vector<std::string> GetRecentProjectList() const override;

  /**
   * @brief Performs internal updates related to project name change.
   */
  void UpdateNames();

private:
  /**
   * @brief Updates the name of the current project on main window.
   */
  void UpdateCurrentProjectName();

  /**
   * @brief Updates recent project list in settings.
   */
  void UpdateRecentProjectNames();

  //!< knows how to interact with the user
  std::unique_ptr<AbstractProjectUserInteractor> m_user_interactor;

  //!< tracks recent projects
  std::unique_ptr<RecentProjectSettings> m_recent_projects;

  //!< performs actual model saving
  std::unique_ptr<IProjectManager> m_project_manager;
};

}  // namespace mvvm

#endif  // MVVM_PROJECT_PROJECT_HANDLER_H_
