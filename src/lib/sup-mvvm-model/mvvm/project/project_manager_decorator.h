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

#ifndef MVVM_PROJECT_PROJECT_MANAGER_DECORATOR_H_
#define MVVM_PROJECT_PROJECT_MANAGER_DECORATOR_H_

#include <mvvm/project/i_project_manager.h>

#include <memory>

namespace mvvm
{

struct ProjectContext;
struct UserInteractionContext;

/**
 * @brief The ProjectManagerDecorator class is an extension of ProjectManager to provide an
 * interaction with the user on open/save-as requests.
 *
 * It relies on the same interface and adds additional logic related to "unsaved" data. For
 * example, on CreateNewProject, it will check if the previous project is saved, and will call
 * external dialog save/discard/cancel via the provided callback.
 */
class MVVM_MODEL_EXPORT ProjectManagerDecorator : public IProjectManager
{
public:
  ProjectManagerDecorator(const ProjectContext& project_context,
                          const UserInteractionContext& user_context);

  ~ProjectManagerDecorator() override;
  ProjectManagerDecorator(const ProjectManagerDecorator& other) = delete;
  ProjectManagerDecorator& operator=(const ProjectManagerDecorator& other) = delete;

  /**
   * @details The directory should exist. If provided name is empty, will call directory selector
   * dialog using callback provided. If current project is in unsaved state, will perform
   * 'save-before-closing' procedure before proceeding further.
   */
  bool CreateNewProject(const std::string& path = {}) override;

  /**
   * @details The project should have a project directory defined to succeed. If it is not the case,
   * it will launch the procedure of directory selection using callback provided.
   */
  bool SaveCurrentProject() override;

  /**
   * @details The directory should exist already. If provided 'dirname' variable is empty, it will
   * acquire a new project directory using dialog provided.
   */
  bool SaveProjectAs(const std::string& path = {}) override;

  /**
   * @details If provided name is empty, will call directory selector dialog using callback
   * provided. If current project is in unsaved state, it will perform 'save-before-closing'
   * procedure before proceeding further.
   */
  bool OpenExistingProject(const std::string& path = {}) override;

  std::string CurrentProjectPath() const override;

  bool IsModified() const override;

  /**
   * @details Will show the dialog, via callback provided, asking the user whether to
   * save/discard/cancel. Returns 'false' only if user has selected 'cancel' button.
   */
  bool CloseCurrentProject() override;

private:
  struct ProjectManagerImpl;
  std::unique_ptr<ProjectManagerImpl> p_impl;
};

}  // namespace mvvm

#endif  // MVVM_PROJECT_PROJECT_MANAGER_DECORATOR_H_
