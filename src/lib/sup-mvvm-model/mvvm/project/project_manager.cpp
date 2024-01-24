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

#include "project_manager.h"

#include "project_types.h"
#include "project_utils.h"

#include <mvvm/interfaces/project_interface.h>

namespace
{
const bool kSucceeded = true;
const bool kFailed = false;
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

  void CreateNewProject() { m_current_project = utils::CreateUntitledProject(m_project_context); }

  /**
   * @brief Returns true if the project has directory already defined.
   */
  bool ProjectHasDir() const { return !m_current_project->GetProjectDir().empty(); }

  /**
   * @brief Saves project in project directory.
   */
  bool SaveCurrentProject() const
  {
    return SaveCurrentProjectAs(m_current_project->GetProjectDir());
  }

  /**
   * @brief Saves the project into a given directory.
   */
  bool SaveCurrentProjectAs(const std::string& dirname) const
  {
    return m_current_project->Save(dirname);
  }

  /**
   * @brief Loads the project from a given directory.
   */
  bool LoadFrom(const std::string& dirname) const { return m_current_project->Load(dirname); }

  /**
   * @brief Returns true if project has been modified after the last save.
   */
  bool IsModified() const { return m_current_project->IsModified(); }
};

ProjectManager::ProjectManager(const ProjectContext& context)
    : p_impl(std::make_unique<ProjectManagerImpl>(context))
{
}

ProjectManager::~ProjectManager() = default;

bool ProjectManager::CreateNewProject(const std::string& dirname)
{
  if (p_impl->IsModified())
  {
    return kFailed;
  }
  p_impl->CreateNewProject();
  return p_impl->SaveCurrentProjectAs(dirname);
}

bool ProjectManager::SaveCurrentProject()
{
  if (!p_impl->ProjectHasDir())
  {
    return kFailed;
  }
  return p_impl->SaveCurrentProject();
}

bool ProjectManager::SaveProjectAs(const std::string& dirname)
{
  return p_impl->SaveCurrentProjectAs(dirname);
}

bool ProjectManager::OpenExistingProject(const std::string& dirname)
{
  if (p_impl->IsModified())
  {
    return kFailed;
  }
  p_impl->CreateNewProject();
  return p_impl->LoadFrom(dirname);
}

std::string ProjectManager::CurrentProjectDir() const
{
  return p_impl->m_current_project ? p_impl->m_current_project->GetProjectDir() : std::string();
}

bool ProjectManager::IsModified() const
{
  return p_impl->IsModified();
}

bool ProjectManager::CloseCurrentProject() const
{
  // no special operation is required to close the project
  p_impl->CreateNewProject();  // ready for further actions
  return kSucceeded;
}

}  // namespace mvvm
