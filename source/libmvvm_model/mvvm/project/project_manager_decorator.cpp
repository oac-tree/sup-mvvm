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

#include "mvvm/project/project_manager_decorator.h"

#include "mvvm/project/project_types.h"
#include "mvvm/project/project_manager.h"

#include <stdexcept>

namespace
{
const bool kSucceeded = true;
const bool kFailed = false;
}  // namespace

namespace mvvm
{

struct ProjectManagerDecorator::ProjectManagerImpl
{
  ProjectContext m_project_context;
  UserInteractionContext m_user_context;
  std::unique_ptr<ProjectManager> project_manager;

  ProjectManagerImpl(ProjectContext project_context, UserInteractionContext user_context)
      : m_project_context(std::move(project_context)), m_user_context(std::move(user_context))
  {
    project_manager = std::make_unique<ProjectManager>(m_project_context);
  }

  //! Returns true if the project has directory already defined.
  bool ProjectHasDir() const { return !project_manager->CurrentProjectDir().empty(); }

  //! Saves project in project directory. If directory is not defined, will acquire
  //! directory susing callback provided.
  bool SaveCurrentProject()
  {
    // Feature FIXME?: already saved project (i.e. isModified=false) will be saved again.
    // Files will be same, but creation date will be changed.

    auto save_dir = ProjectHasDir() ? project_manager->CurrentProjectDir() : AcquireNewProjectDir();
    return SaveCurrentProjectAs(save_dir);
  }

  //! Saves current project under directory selected.
  bool SaveCurrentProjectAs(const std::string& dirname) const
  {
    // empty dirname varible means 'cancel' during directory selection
    return dirname.empty() ? kFailed : project_manager->SaveProjectAs(dirname);
  }

  std::string CurrentProjectDir() const { return project_manager->CurrentProjectDir(); }

  bool IsModified() const { return project_manager->IsModified(); }

  //! Performs saving of previous project before creating a new one.
  bool SaveBeforeClosing()
  {
    if (IsModified())
    {
      switch (AcquireSaveChangesAnswer())
      {
      case SaveChangesAnswer::kSave:
        return SaveCurrentProject();
      case SaveChangesAnswer::kCancel:
        return kFailed;  // saving was interrupted by the 'cancel' button
      case SaveChangesAnswer::kDiscard:
        project_manager->CloseCurrentProject();
        return kSucceeded;
      default:
        throw std::runtime_error("Error in ProjectManager: unexpected answer.");
      }
    }
    return kSucceeded;
  }

  //! Asks the user whether to save/cancel/discard the project using callback provided.
  SaveChangesAnswer AcquireSaveChangesAnswer() const
  {
    if (!m_user_context.m_answer_callback)
    {
      throw std::runtime_error("Error in ProjectManager: absent save_callback");
    }
    return m_user_context.m_answer_callback();
  }

  //! Acquire the name of the new project directory using callback provided.
  std::string AcquireNewProjectDir()
  {
    if (!m_user_context.m_create_dir_callback)
    {
      throw std::runtime_error("Error in ProjectManager: absent creat_dir callback.");
    }
    return m_user_context.m_create_dir_callback();
  }

  //! Acquire the name of the existing project directory using callback provided.
  std::string AcquireExistingProjectDir()
  {
    if (!m_user_context.m_select_dir_callback)
    {
      throw std::runtime_error("Error in ProjectManager: absent open_dir callback.");
    }
    return m_user_context.m_select_dir_callback();
  }
};

//! Constructor for ProjectManagerDecorator.

ProjectManagerDecorator::ProjectManagerDecorator(const ProjectContext& project_context,
                                                 const UserInteractionContext& user_context)
    : p_impl(std::make_unique<ProjectManagerImpl>(project_context, user_context))
{
}

ProjectManagerDecorator::~ProjectManagerDecorator() = default;

//! Creates a new project in the directory 'dirname', returns 'true' in the case of success.
//! The directory should exist.
//! If provided name is empty, will call directory selector dialog using callback provided.
//! If current project is in unsaved state, will perform 'save-before-closing' procedure before
//! proceeding further.

bool ProjectManagerDecorator::CreateNewProject(const std::string& dirname)
{
  if (!p_impl->SaveBeforeClosing())
  {
    return kFailed;
  }

  auto project_dir = dirname.empty() ? p_impl->AcquireNewProjectDir() : dirname;
  // empty project_dir string denotes 'cancel' during directory creation dialog
  return project_dir.empty() ? kFailed : p_impl->project_manager->CreateNewProject(project_dir);
}

//! Saves current project, returns 'true' in the case of success.
//! The project should have a project directory defined, if it is not the case, it will
//! launch the procedure of directory selection using callback provided.

bool ProjectManagerDecorator::SaveCurrentProject()
{
  return p_impl->SaveCurrentProject();
}

//! Saves the project under a given directory, returns true in the case of success.
//! The directory should exist already. If provided 'dirname' variable is empty,
//! it will acquire a new project directory using dialog provided.

bool ProjectManagerDecorator::SaveProjectAs(const std::string& dirname)
{
  auto project_dir = dirname.empty() ? p_impl->AcquireNewProjectDir() : dirname;
  // empty project_dir variable denotes 'cancel' during directory creation dialog
  return project_dir.empty() ? kFailed : p_impl->SaveCurrentProjectAs(project_dir);
}

//! Opens existing project, returns 'true' in the case of success.
//! If provided name is empty, will call directory selector dialog using callback provided.
//! If current project is in unsaved state, it will perform 'save-before-closing' procedure before
//! proceeding further.

bool ProjectManagerDecorator::OpenExistingProject(const std::string& dirname)
{
  if (!p_impl->SaveBeforeClosing())
  {
    return kFailed;
  }
  auto project_dir = dirname.empty() ? p_impl->AcquireExistingProjectDir() : dirname;
  // empty project_dir variable denotes 'cancel' during directory selection dialog
  return project_dir.empty() ? kFailed : p_impl->project_manager->OpenExistingProject(project_dir);
}

//! Returns current project directory.

std::string ProjectManagerDecorator::CurrentProjectDir() const
{
  return p_impl->CurrentProjectDir();
}

//! Returns true if project was modified since last save.

bool ProjectManagerDecorator::IsModified() const
{
  return p_impl->IsModified();
}

//! Closes current project, returns 'true' if succeeded.
//! Will show the dialog, via callback provided, asking the user whether to save/discard/cancel.
//! Returns 'false' only if user has selected 'cancel' button.

bool ProjectManagerDecorator::CloseCurrentProject() const
{
  if (!p_impl->SaveBeforeClosing())
  {
    return kFailed;
  }
  return kSucceeded;
}

}  // namespace mvvm
