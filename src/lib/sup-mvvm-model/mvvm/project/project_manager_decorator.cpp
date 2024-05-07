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

#include "project_manager_decorator.h"

#include <mvvm/core/exceptions.h>

namespace
{
const bool kSucceeded = true;
const bool kFailed = false;
}  // namespace

namespace mvvm
{

ProjectManagerDecorator::ProjectManagerDecorator(std::unique_ptr<IProjectManager> decoratee,
                                                 UserInteractionContext user_context)
    : m_project_manager(std::move(decoratee)), m_user_context(std::move(user_context))
{
}

ProjectManagerDecorator::~ProjectManagerDecorator() = default;

bool ProjectManagerDecorator::CreateNewProject(const std::string& path)
{
  if (!SaveBeforeClosing())
  {
    return kFailed;
  }

  auto project_dir = path.empty() ? AcquireNewProjectPath() : path;
  // empty project_dir string denotes 'cancel' during directory creation dialog
  return project_dir.empty() ? kFailed : m_project_manager->CreateNewProject(project_dir);
}

bool ProjectManagerDecorator::SaveCurrentProject()
{
  return SaveProjectAs(CurrentProjectPath());
}

bool ProjectManagerDecorator::SaveProjectAs(const std::string& path)
{
  auto project_dir = path.empty() ? AcquireNewProjectPath() : path;
  // empty project_dir variable denotes 'cancel' during directory creation dialog
  return project_dir.empty() ? kFailed : m_project_manager->SaveProjectAs(project_dir);
}

bool ProjectManagerDecorator::OpenExistingProject(const std::string& path)
{
  if (!SaveBeforeClosing())
  {
    return kFailed;
  }
  auto project_dir = path.empty() ? AcquireExistingProjectPath() : path;
  // empty project_dir variable denotes 'cancel' during directory selection dialog
  return project_dir.empty() ? kFailed : m_project_manager->OpenExistingProject(project_dir);
}

std::string ProjectManagerDecorator::CurrentProjectPath() const
{
  return m_project_manager->CurrentProjectPath();
}

bool ProjectManagerDecorator::IsModified() const
{
  return m_project_manager->IsModified();
}

bool ProjectManagerDecorator::CloseCurrentProject()
{
  return SaveBeforeClosing() ? kSucceeded : kFailed;
}

bool ProjectManagerDecorator::ProjectHasPath() const
{
  return !m_project_manager->CurrentProjectPath().empty();
}

bool ProjectManagerDecorator::SaveBeforeClosing()
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
      m_project_manager->CloseCurrentProject();
      return kSucceeded;
    default:
      throw RuntimeException("Error in ProjectManager: unexpected answer.");
    }
  }
  return kSucceeded;
}

SaveChangesAnswer ProjectManagerDecorator::AcquireSaveChangesAnswer() const
{
  if (!m_user_context.m_answer_callback)
  {
    throw RuntimeException("Error in ProjectManager: absent save_callback");
  }
  return m_user_context.m_answer_callback();
}

std::string ProjectManagerDecorator::AcquireNewProjectPath() const
{
  if (!m_user_context.m_create_dir_callback)
  {
    throw RuntimeException("Error in ProjectManager: absent creat_dir callback.");
  }
  return m_user_context.m_create_dir_callback();
}

std::string ProjectManagerDecorator::AcquireExistingProjectPath() const
{
  if (!m_user_context.m_select_dir_callback)
  {
    throw RuntimeException("Error in ProjectManager: absent open_dir callback.");
  }
  return m_user_context.m_select_dir_callback();
}

}  // namespace mvvm
