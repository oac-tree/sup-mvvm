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

#include "i_project.h"

#include <mvvm/core/exceptions.h>

namespace
{
const bool kSucceeded = true;
const bool kFailed = false;
}  // namespace

namespace mvvm
{

ProjectManager::ProjectManager(create_project_t create_project)
    : m_create_project_callback(std::move(create_project))
{
  CreateUntitledProject();
}

ProjectManager::~ProjectManager() = default;

bool ProjectManager::CreateNewProject(const std::string& path)
{
  if (m_current_project->IsModified())
  {
    return kFailed;
  }

  return m_current_project->Save(path);
}

bool ProjectManager::SaveCurrentProject()
{
  if (!ProjectHasPath())
  {
    return kFailed;
  }

  return SaveProjectAs(m_current_project->GetProjectPath());
}

bool ProjectManager::SaveProjectAs(const std::string& path)
{
  return m_current_project->Save(path);
}

bool ProjectManager::OpenExistingProject(const std::string& path)
{
  if (m_current_project->IsModified())
  {
    return kFailed;
  }

  return m_current_project->Load(path);
}

std::string ProjectManager::CurrentProjectPath() const
{
  return m_current_project->GetProjectPath();
}

bool ProjectManager::IsModified() const
{
  return m_current_project->IsModified();
}

bool ProjectManager::CloseCurrentProject()
{
  // no special operation is required to close the project
  CreateUntitledProject();
  return kSucceeded;
}

void ProjectManager::CreateUntitledProject()
{
  if (!m_create_project_callback)
  {
    throw RuntimeException("Can't create project, no callback specified");
  }

  m_current_project = m_create_project_callback();

  if (!m_current_project)
  {
    throw RuntimeException("Can't create project");
  }
}

bool ProjectManager::ProjectHasPath()
{
  return !m_current_project->GetProjectPath().empty();
}

}  // namespace mvvm
