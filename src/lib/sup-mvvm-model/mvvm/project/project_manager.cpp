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

namespace mvvm
{

namespace
{
const bool kSucceeded = true;
const bool kFailed = false;

}  // namespace

ProjectManager::ProjectManager(IProject* project_agent,
                                                 UserInteractionContext user_context)
    : m_project_manager(project_agent), m_user_context(std::move(user_context))
{
  if (!m_user_context.answer_callback)
  {
    throw RuntimeException("Error in ProjectManager: absent answer_callback");
  }

  if (!m_user_context.new_path_callback)
  {
    throw RuntimeException("Error in ProjectManager: absent new_path_callback");
  }

  if (!m_user_context.existing_path_callback)
  {
    throw RuntimeException("Error in ProjectManager: absent existing_path_callback");
  }

  if (!m_user_context.message_callback)
  {
    throw RuntimeException("Error in ProjectManager: absent message_callback");
  }
}

ProjectManager::~ProjectManager() = default;

std::string ProjectManager::GetProjectPath() const
{
  return GetProject()->GetProjectPath();
}

bool ProjectManager::IsModified() const
{
  return GetProject()->IsModified();
}

bool ProjectManager::CreateNewProject(const std::string& path)
{
  if (!SaveBeforeClosing())
  {
    return kFailed;
  }

  auto project_dir = path.empty() ? AcquireNewProjectPath() : path;

  if (project_dir.empty())
  {
    // empty project_dir string denotes 'cancel' during directory creation dialog
    return kFailed;
  }

  GetProject()->Close();
  GetProject()->CreateEmpty();

  return GetProject()->Save(project_dir);
}

bool ProjectManager::CloseProject()
{
  if (!SaveBeforeClosing())
  {
    return kFailed;
  }

  return GetProject()->Close();
}

bool ProjectManager::SaveCurrentProject()
{
  return SaveProjectAs(GetProjectPath());
}

bool ProjectManager::SaveProjectAs(const std::string& path)
{
  auto project_dir = path.empty() ? AcquireNewProjectPath() : path;
  // empty project_dir variable denotes 'cancel' during directory creation dialog
  return project_dir.empty() ? kFailed : GetProject()->Save(project_dir);
}

bool ProjectManager::OpenExistingProject(const std::string& path)
{
  if (!SaveBeforeClosing())
  {
    return kFailed;
  }

  auto project_dir = path.empty() ? AcquireExistingProjectPath() : path;
  if (project_dir.empty())
  {
    // Empty project_dir variable denotes 'cancel' during project path selection dialog.
    return kFailed;
  }

  try
  {
    return GetProject()->Load(project_dir);
  }
  catch (const std::exception& ex)
  {
    // XML parsing error
    m_user_context.message_callback(ex.what());
    return kFailed;
  }
}

IProject* ProjectManager::GetProject() const
{
  return m_project_manager;
}

bool ProjectManager::SaveBeforeClosing()
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
      return kSucceeded;
    default:
      throw RuntimeException("Error in ProjectManager: unexpected answer.");
    }
  }
  return kSucceeded;
}

SaveChangesAnswer ProjectManager::AcquireSaveChangesAnswer() const
{
  return m_user_context.answer_callback();
}

std::string ProjectManager::AcquireNewProjectPath() const
{
  return m_user_context.new_path_callback();
}

std::string ProjectManager::AcquireExistingProjectPath() const
{
  return m_user_context.existing_path_callback();
}

}  // namespace mvvm
