/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/project/projectmanager.h"

#include "mvvm/interfaces/projectinterface.h"
#include "mvvm/project/project_types.h"
#include "mvvm/project/projectutils.h"

namespace
{
const bool succeeded = true;
const bool failed = false;
}  // namespace

namespace mvvm
{

struct ProjectManager::ProjectManagerImpl
{
  std::unique_ptr<ProjectInterface> m_current_project;
  ProjectContext m_project_context;

  explicit ProjectManagerImpl(ProjectContext context) : m_project_context(std::move(context))
  {
    CreateNewProject();
  }

  //! Creates new project. Closes current project. Used in assumption that project was already
  //! saved.
  void CreateNewProject()
  {
    m_current_project = ProjectUtils::CreateUntitledProject(m_project_context);
  }

  //! Returns true if the project has directory already defined.
  bool ProjectHasDir() const { return !m_current_project->GetProjectDir().empty(); }

  //! Saves project in project directory. If directory is not defined
  bool SaveCurrentProject() const
  {
    return SaveCurrentProjectAs(m_current_project->GetProjectDir());
  }

  //! Saves the project into a given directory.
  bool SaveCurrentProjectAs(const std::string& dirname) const
  {
    return m_current_project->Save(dirname);
  }

  //! Loads the project from a given directory.
  bool LoadFrom(const std::string& dirname) const { return m_current_project->Load(dirname); }

  //! Returns true if project has been modified after the last save.
  bool IsModified() const { return m_current_project->IsModified(); }
};

//! Constructor for ProjectManager.

ProjectManager::ProjectManager(const ProjectContext& context)
    : p_impl(std::make_unique<ProjectManagerImpl>(context))
{
}

ProjectManager::~ProjectManager() = default;

//! Creates a new project, returns 'true' in the case of success.
//! Current project has to be in a saved state, otherwise will return false.

bool ProjectManager::CreateNewProject(const std::string& dirname)
{
  if (p_impl->IsModified())
  {
    return failed;
  }
  p_impl->CreateNewProject();
  return p_impl->SaveCurrentProjectAs(dirname);
}

//! Saves current project, returns 'true' in the case of success.
//! The project should have a project directory defined to succeed.

bool ProjectManager::SaveCurrentProject()
{
  if (!p_impl->ProjectHasDir())
  {
    return failed;
  }
  return p_impl->SaveCurrentProject();
}

//! Saves the project under a given directory, returns true in the case of success.
//! The directory should exist already.

bool ProjectManager::SaveProjectAs(const std::string& dirname)
{
  return p_impl->SaveCurrentProjectAs(dirname);
}

//! Opens existing project, returns 'true' in the case of success.
//! Current project should be in a saved state, new project should exist.

bool ProjectManager::OpenExistingProject(const std::string& dirname)
{
  if (p_impl->IsModified())
  {
    return failed;
  }
  p_impl->CreateNewProject();
  return p_impl->LoadFrom(dirname);
}

//! Returns current project directory.

std::string ProjectManager::CurrentProjectDir() const
{
  return p_impl->m_current_project ? p_impl->m_current_project->GetProjectDir() : std::string();
}

//! Returns true if project was modified since last save.

bool ProjectManager::IsModified() const
{
  return p_impl->IsModified();
}

//! Closes current project (without saving).
//! No checks whether it is modified or not being performed.

bool ProjectManager::CloseCurrentProject() const
{
  // no special operation is required to close the project
  p_impl->CreateNewProject();  // ready for further actions
  return succeeded;
}

}  // namespace mvvm
