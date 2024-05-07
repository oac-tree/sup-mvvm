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

#include "i_project.h"
#include "project_context.h"
#include "project_manager.h"
#include "project_utils.h"

#include <mvvm/core/exceptions.h>

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
  std::unique_ptr<IProjectManager> m_project_manager;

  ProjectManagerImpl(ProjectContext project_context, UserInteractionContext user_context)
      : m_project_context(std::move(project_context)), m_user_context(std::move(user_context))
  {
    auto project_func = [this]() -> std::unique_ptr<IProject>
    { return mvvm::utils::CreateUntitledFolderBasedProject(m_project_context); };

    m_project_manager = std::make_unique<ProjectManager>(project_func);
  }

  ProjectManagerImpl(std::unique_ptr<IProjectManager> decoratee,
                     UserInteractionContext user_context)
      : m_user_context(std::move(user_context)), m_project_manager(std::move(decoratee))
  {
  }

  /**
   * @brief Returns true if the project has directory already defined.
   */
  bool ProjectHasPath() const { return !m_project_manager->CurrentProjectPath().empty(); }

  /**
   * @brief Saves project in project directory. If directory is not defined, will acquire directory
   * susing callback provided.
   */
  bool SaveCurrentProject()
  {
    // Feature FIXME?: already saved project (i.e. isModified=false) will be saved again.
    // Files will be same, but creation date will be changed.

    auto save_dir =
        ProjectHasPath() ? m_project_manager->CurrentProjectPath() : AcquireNewProjectPath();
    return SaveCurrentProjectAs(save_dir);
  }

  /**
   * @brief Saves current project under directory selected.
   */
  bool SaveCurrentProjectAs(const std::string& path) const
  {
    // empty dirname varible means 'cancel' during directory selection
    return path.empty() ? kFailed : m_project_manager->SaveProjectAs(path);
  }

  std::string CurrentProjectDir() const { return m_project_manager->CurrentProjectPath(); }

  bool IsModified() const { return m_project_manager->IsModified(); }

  /**
   * @brief Performs saving of previous project before creating a new one.
   */
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
        m_project_manager->CloseCurrentProject();
        return kSucceeded;
      default:
        throw RuntimeException("Error in ProjectManager: unexpected answer.");
      }
    }
    return kSucceeded;
  }

  /**
   * @brief Asks the user whether to save/cancel/discard the project using callback provided.
   */
  SaveChangesAnswer AcquireSaveChangesAnswer() const
  {
    if (!m_user_context.m_answer_callback)
    {
      throw RuntimeException("Error in ProjectManager: absent save_callback");
    }
    return m_user_context.m_answer_callback();
  }

  /**
   * @brief Acquire the name of the new project directory using callback provided.
   */
  std::string AcquireNewProjectPath()
  {
    if (!m_user_context.m_create_dir_callback)
    {
      throw RuntimeException("Error in ProjectManager: absent creat_dir callback.");
    }
    return m_user_context.m_create_dir_callback();
  }

  /**
   * @brief Acquire the name of the existing project directory using callback provided.
   */
  std::string AcquireExistingProjectPath()
  {
    if (!m_user_context.m_select_dir_callback)
    {
      throw RuntimeException("Error in ProjectManager: absent open_dir callback.");
    }
    return m_user_context.m_select_dir_callback();
  }
};

ProjectManagerDecorator::ProjectManagerDecorator(std::unique_ptr<IProjectManager> decoratee,
                                                 const UserInteractionContext& user_context)
    : p_impl(std::make_unique<ProjectManagerImpl>(std::move(decoratee), user_context))
{
}

ProjectManagerDecorator::ProjectManagerDecorator(const ProjectContext& project_context,
                                                 const UserInteractionContext& user_context)
    : p_impl(std::make_unique<ProjectManagerImpl>(project_context, user_context))
{
}

ProjectManagerDecorator::~ProjectManagerDecorator() = default;

bool ProjectManagerDecorator::CreateNewProject(const std::string& path)
{
  if (!p_impl->SaveBeforeClosing())
  {
    return kFailed;
  }

  auto project_dir = path.empty() ? p_impl->AcquireNewProjectPath() : path;
  // empty project_dir string denotes 'cancel' during directory creation dialog
  return project_dir.empty() ? kFailed : p_impl->m_project_manager->CreateNewProject(project_dir);
}

bool ProjectManagerDecorator::SaveCurrentProject()
{
  return p_impl->SaveCurrentProject();
}

bool ProjectManagerDecorator::SaveProjectAs(const std::string& path)
{
  auto project_dir = path.empty() ? p_impl->AcquireNewProjectPath() : path;
  // empty project_dir variable denotes 'cancel' during directory creation dialog
  return project_dir.empty() ? kFailed : p_impl->SaveCurrentProjectAs(project_dir);
}

bool ProjectManagerDecorator::OpenExistingProject(const std::string& path)
{
  if (!p_impl->SaveBeforeClosing())
  {
    return kFailed;
  }
  auto project_dir = path.empty() ? p_impl->AcquireExistingProjectPath() : path;
  // empty project_dir variable denotes 'cancel' during directory selection dialog
  return project_dir.empty() ? kFailed
                             : p_impl->m_project_manager->OpenExistingProject(project_dir);
}

std::string ProjectManagerDecorator::CurrentProjectPath() const
{
  return p_impl->CurrentProjectDir();
}

bool ProjectManagerDecorator::IsModified() const
{
  return p_impl->IsModified();
}

bool ProjectManagerDecorator::CloseCurrentProject()
{
  if (!p_impl->SaveBeforeClosing())
  {
    return kFailed;
  }
  return kSucceeded;
}

}  // namespace mvvm
