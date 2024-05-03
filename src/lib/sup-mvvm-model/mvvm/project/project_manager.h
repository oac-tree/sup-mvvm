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

#ifndef MVVM_PROJECT_PROJECT_MANAGER_H_
#define MVVM_PROJECT_PROJECT_MANAGER_H_

#include <mvvm/project/i_project_manager.h>

#include <memory>

namespace mvvm
{

struct ProjectContext;

/**
 * @brief The ProjectManager class is responsible for handling new/save/save-as/close project logic.
 *
 * This ProjectManager requires certain prerequisites to function properly: for example, the
 * creation of a new project will be possible only if the old project is in a saved state. See the
 * description of the class methods. The ProjectManager shall not be used directly. Use
 * ProjectManagerDecorator instead, which provides additional safety checks, and necessary
 * interactions with the user.
 */
class MVVM_MODEL_EXPORT ProjectManager : public IProjectManager
{
public:
  explicit ProjectManager(const ProjectContext& context);
  ~ProjectManager() override;

  ProjectManager(const ProjectManager& other) = delete;
  ProjectManager& operator=(const ProjectManager& other) = delete;

  /**
   * @details Call to this method will first close current project. So it is assumed that current
   * project was already saved, otherwise will return false.
   */
  bool CreateNewProject(const std::string& path) override;

  /**
   * @details The project should have a project directory defined to succeed.
   */
  bool SaveCurrentProject() override;

  /**
   * @details The directory should exist already.
   */
  bool SaveProjectAs(const std::string& path) override;

  /**
   * @details Current project should be in a saved state, new project should exist.
   */
  bool OpenExistingProject(const std::string& path) override;

  std::string CurrentProjectPath() const override;

  bool IsModified() const override;

  bool CloseCurrentProject() const override;

private:
  struct ProjectManagerImpl;
  std::unique_ptr<ProjectManagerImpl> p_impl;
};

}  // namespace mvvm

#endif  // MVVM_PROJECT_PROJECT_MANAGER_H_
