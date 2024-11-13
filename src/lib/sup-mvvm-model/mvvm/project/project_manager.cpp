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

ProjectManager::ProjectManager(IProject* project_agent) : m_project_agent(project_agent)
{
  if (!m_project_agent)
  {
    throw RuntimeException("Uninitialized project agent");
  }
}

ProjectManager::~ProjectManager() = default;

bool ProjectManager::CreateNewProject(const std::string& path)
{
  if (m_project_agent->IsModified())
  {
    return kFailed;
  }

  m_project_agent->CreateNewProject();

  return m_project_agent->Save(path);
}

bool ProjectManager::SaveCurrentProject()
{
  if (!ProjectHasPath())
  {
    return kFailed;
  }

  return SaveProjectAs(m_project_agent->GetProjectPath());
}

bool ProjectManager::SaveProjectAs(const std::string& path)
{
  return m_project_agent->Save(path);
}

bool ProjectManager::OpenExistingProject(const std::string& path)
{
  if (m_project_agent->IsModified())
  {
    return kFailed;
  }

  return m_project_agent->Load(path);
}

std::string ProjectManager::CurrentProjectPath() const
{
  return m_project_agent->GetProjectPath();
}

bool ProjectManager::IsModified() const
{
  return m_project_agent->IsModified();
}

bool ProjectManager::CloseCurrentProject()
{
  return m_project_agent->CloseProject();
}

IProject* ProjectManager::GetProject() const
{
  return m_project_agent;
}

bool ProjectManager::ProjectHasPath()
{
  return !m_project_agent->GetProjectPath().empty();
}

}  // namespace mvvm
