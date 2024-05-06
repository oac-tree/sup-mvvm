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

#include "new_project_manager.h"

#include "i_project.h"
#include "project_context.h"
#include "project_utils.h"

namespace
{
const bool kSucceeded = true;
const bool kFailed = false;
}  // namespace

namespace mvvm
{

struct NewProjectManager::NewProjectManagerImpl
{
  std::unique_ptr<IProject> m_current_project;
  ProjectContext m_project_context;

  explicit NewProjectManagerImpl(ProjectContext context) : m_project_context(std::move(context))
  {
    CreateNewProject();
  }

  void CreateNewProject() { m_current_project = utils::CreateUntitledProject(m_project_context); }

  /**
   * @brief Returns true if the project has directory already defined.
   */
  bool ProjectHasDir() const { return !m_current_project->GetProjectPath().empty(); }

  /**
   * @brief Saves project in project directory.
   */
  bool SaveCurrentProject() const
  {
    return SaveCurrentProjectAs(m_current_project->GetProjectPath());
  }

  /**
   * @brief Saves the project into a given directory.
   */
  bool SaveCurrentProjectAs(const std::string& path) const
  {
    return m_current_project->Save(path);
  }

  /**
   * @brief Loads the project from a given directory.
   */
  bool LoadFrom(const std::string& path) const { return m_current_project->Load(path); }

  /**
   * @brief Returns true if project has been modified after the last save.
   */
  bool IsModified() const { return m_current_project->IsModified(); }
};

NewProjectManager::NewProjectManager(const ProjectContext& context)
    : p_impl(std::make_unique<NewProjectManagerImpl>(context))
{
}

NewProjectManager::~NewProjectManager() = default;

bool NewProjectManager::CreateNewProject(const std::string& path)
{
  if (p_impl->IsModified())
  {
    return kFailed;
  }
  p_impl->CreateNewProject();
  return p_impl->SaveCurrentProjectAs(path);
}

bool NewProjectManager::SaveCurrentProject()
{
  if (!p_impl->ProjectHasDir())
  {
    return kFailed;
  }
  return p_impl->SaveCurrentProject();
}

bool NewProjectManager::SaveProjectAs(const std::string& path)
{
  return p_impl->SaveCurrentProjectAs(path);
}

bool NewProjectManager::OpenExistingProject(const std::string& path)
{
  if (p_impl->IsModified())
  {
    return kFailed;
  }
  p_impl->CreateNewProject();
  return p_impl->LoadFrom(path);
}

std::string NewProjectManager::CurrentProjectPath() const
{
  return p_impl->m_current_project ? p_impl->m_current_project->GetProjectPath() : std::string();
}

bool NewProjectManager::IsModified() const
{
  return p_impl->IsModified();
}

bool NewProjectManager::CloseCurrentProject() const
{
  // no special operation is required to close the project
  p_impl->CreateNewProject();  // ready for further actions
  return kSucceeded;
}

}  // namespace mvvm
