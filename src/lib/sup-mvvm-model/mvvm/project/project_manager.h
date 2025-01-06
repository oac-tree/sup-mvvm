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

#ifndef MVVM_PROJECT_PROJECT_MANAGER_H_
#define MVVM_PROJECT_PROJECT_MANAGER_H_

#include <mvvm/project/i_project_manager.h>
#include <mvvm/project/project_context.h>

namespace mvvm
{

struct UserInteractionContext;
class IProject;

/**
 * @brief The ProjectManager class assists the user in saving projects on disk and loading it from
 * there.
 *
 * It holds the logic related to "unsaved" data. For example, on CreateNewProject, it will check if
 * the previous project is saved, and will call external dialog save/discard/cancel via the provided
 * callback.
 */
class MVVM_MODEL_EXPORT ProjectManager : public IProjectManager
{
public:
  ProjectManager(IProject* project, UserInteractionContext user_context);

  ~ProjectManager() override;
  ProjectManager(const ProjectManager& other) = delete;
  ProjectManager& operator=(const ProjectManager& other) = delete;

  /**
   * @details If the path is empty will call a dialog for folder selection (for folder-based
   * projects) or file selection (for file-based projects). If the current project is unsaved, will
   * perform 'save-before-closing' procedure before proceeding further.
   */
  bool CreateNewProject(const std::string& path) override;

  /**
   * @details Will show the dialog, via callback provided, asking the user whether to
   * save/discard/cancel. Returns 'false' only if user has selected 'cancel' button.
   */
  bool CloseProject() override;

  /**
   * @details The project should have a path defined for success. If this is not the case, it will
   * launch a dialog to select a folder (for folder-based projects) or file (for file-based
   * projects) using callback provided.
   */
  bool SaveCurrentProject() override;

  /**
   * @details If the path variable is non-empty, then it should exist. If the provided path is
   * empty, it will launch a dialog to select a folder (for folder-based projects) or file (for
   * file-based projects) using callback provided.
   */
  bool SaveProjectAs(const std::string& path) override;

  /**
   * @details If the provided path is empty, it will launch a dialog to select a folder (for
   * folder-based projects) or file (for file-based projects) using callback provided. If current
   * project is in unsaved state, it will perform 'save-before-closing' procedure before proceeding
   * further.
   */
  bool OpenExistingProject(const std::string& path) override;

  IProject* GetProject() const override;

  void ClearRecentProjectsList() override;

  std::vector<std::string> GetRecentProjectList() const override;

private:
  /**
   * @brief Performs saving of previous project before creating a new one.
   */
  bool SaveBeforeClosing();

  /**
   * @brief Asks the user whether to save/cancel/discard the project using callback provided.
   */
  SaveChangesAnswer AcquireSaveChangesAnswer() const;

  /**
   * @brief Acquire the name of the new project path using callback provided.
   */
  std::string AcquireNewProjectPath() const;

  /**
   * @brief Acquire the name of the existing project path using callback provided.
   */
  std::string AcquireExistingProjectPath() const;

  IProject* m_project{nullptr};
  UserInteractionContext m_user_context;
};

}  // namespace mvvm

#endif  // MVVM_PROJECT_PROJECT_MANAGER_H_
