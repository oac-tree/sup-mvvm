/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
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

#include <mvvm/interfaces/project_manager_interface.h>

#include <memory>

namespace mvvm
{

struct ProjectContext;
struct UserInteractionContext;

//! Decorator for ProjectManager to provide interaction with the user on open/save-as requests.
//! It relies on the same interface and adds additional logic related to "unsaved" data.

//! For example, on createNewProject, it will check if the previous project is saved, and will
//! call external dialog save/discard/cancel via the provided callback.

class MVVM_MODEL_EXPORT ProjectManagerDecorator : public ProjectManagerInterface
{
public:
  ProjectManagerDecorator(const ProjectContext& project_context,
                          const UserInteractionContext& user_context);

  ~ProjectManagerDecorator() override;
  ProjectManagerDecorator(const ProjectManagerDecorator& other) = delete;
  ProjectManagerDecorator& operator=(const ProjectManagerDecorator& other) = delete;

  bool CreateNewProject(const std::string& dirname = {}) override;

  bool SaveCurrentProject() override;

  bool SaveProjectAs(const std::string& dirname = {}) override;

  bool OpenExistingProject(const std::string& dirname = {}) override;

  std::string CurrentProjectDir() const override;

  bool IsModified() const override;

  bool CloseCurrentProject() const override;

private:
  struct ProjectManagerImpl;
  std::unique_ptr<ProjectManagerImpl> p_impl;
};

}  // namespace mvvm

#endif  // MVVM_PROJECT_PROJECT_MANAGER_DECORATOR_H_
