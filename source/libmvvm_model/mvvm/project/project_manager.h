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

#ifndef MVVM_PROJECT_PROJECT_MANAGER_H_
#define MVVM_PROJECT_PROJECT_MANAGER_H_

#include <mvvm/interfaces/project_manager_interface.h>

#include <memory>

namespace mvvm
{

struct ProjectContext;

//! Responsible for handling new/save/save-as/close Project logic, where the Project represents
//! a collection of serialized application models in the project directory.

//! This ProjectManager requires certain prerequisites to function properly: for example,
//! the creation of a new project will be possible only if the old project is in a saved state. See
//! description to the class methods.

class MVVM_MODEL_EXPORT ProjectManager : public ProjectManagerInterface
{
public:
  explicit ProjectManager(const ProjectContext& context);
  ~ProjectManager() override;

  ProjectManager(const ProjectManager& other) = delete;
  ProjectManager& operator=(const ProjectManager& other) = delete;

  bool CreateNewProject(const std::string& dirname) override;

  bool SaveCurrentProject() override;

  bool SaveProjectAs(const std::string& dirname) override;

  bool OpenExistingProject(const std::string& dirname) override;

  std::string CurrentProjectDir() const override;

  bool IsModified() const override;

  bool CloseCurrentProject() const override;

private:
  struct ProjectManagerImpl;
  std::unique_ptr<ProjectManagerImpl> p_impl;
};

}  // namespace mvvm

#endif  // MVVM_PROJECT_PROJECT_MANAGER_H_
