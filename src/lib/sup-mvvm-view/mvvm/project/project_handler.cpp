/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "project_handler.h"

#include "file_based_user_interactor.h"
#include "folder_based_user_interactor.h"
#include "recent_project_settings.h"

#include <mvvm/core/exceptions.h>
#include <mvvm/project/i_project.h>
#include <mvvm/project/project_context.h>
#include <mvvm/project/project_manager_factory.h>
#include <mvvm/project/project_utils.h>
#include <mvvm/viewmodel/qtcore_helper.h>
#include <mvvm/widgets/widget_utils.h>

#include <QMainWindow>

namespace mvvm
{

namespace
{

/**
 * @brief Returns user interactor corresponding to the given project.
 *
 * It will provide dialogs to open existing projects, or create new.
 */
std::unique_ptr<AbstractProjectUserInteractor> CreateUserInteractor(IProject *project)
{
  auto application_type = QString::fromStdString(project->GetApplicationType());

  if (project->GetProjectType() == ProjectType::kFolderBased)
  {
    return std::make_unique<FolderBasedUserInteractor>(application_type, utils::FindMainWindow());
  }

  if (project->GetProjectType() == ProjectType::kFileBased)
  {
    return std::make_unique<FileBasedUserInteractor>(application_type, utils::FindMainWindow());
  }

  throw RuntimeException("Unknown project type");
}

}  // namespace

ProjectHandler::ProjectHandler(IProject *project)
    : m_user_interactor(CreateUserInteractor(project))
    , m_recent_projects(std::make_unique<RecentProjectSettings>())
    , m_project_manager(CreateProjectManager(*project, m_user_interactor->CreateContext()))
{
  UpdateNames();
  m_user_interactor->SetCurrentWorkdir(m_recent_projects->GetCurrentWorkdir().toStdString());
}

ProjectHandler::~ProjectHandler()
{
  m_recent_projects->SetCurrentWorkdir(
      QString::fromStdString(m_user_interactor->GetCurrentWorkdir()));
}

bool ProjectHandler::CreateNewProject(const std::string &path)
{
  return m_project_manager->CreateNewProject(path);
}

bool ProjectHandler::CloseProject()
{
  return m_project_manager->CloseProject();
}

bool ProjectHandler::SaveCurrentProject()
{
  return m_project_manager->SaveCurrentProject();
}

bool ProjectHandler::SaveProjectAs(const std::string &path)
{
  return m_project_manager->SaveProjectAs(path);
}

bool ProjectHandler::OpenExistingProject(const std::string &path)
{
  return m_project_manager->OpenExistingProject(path);
}

IProject *ProjectHandler::GetProject() const
{
  return m_project_manager->GetProject();
}

void ProjectHandler::ClearRecentProjectsList()
{
  m_recent_projects->ClearRecentProjectsList();
  UpdateNames();
}

std::vector<std::string> ProjectHandler::GetRecentProjectList() const
{
  return utils::GetStdStringVector(m_recent_projects->GetRecentProjectList());
}

void ProjectHandler::UpdateNames()
{
  UpdateCurrentProjectName();
  UpdateRecentProjectNames();
}

void ProjectHandler::UpdateCurrentProjectName()
{
  const auto current_project_path = m_project_manager->GetProject()->GetPath();
  const auto is_modified = m_project_manager->GetProject()->IsModified();

  // set main window title
  auto title = utils::ProjectWindowTitle(current_project_path, is_modified);
  if (auto main_window = utils::FindMainWindow(); main_window)
  {
    main_window->setWindowTitle(QString::fromStdString(title));
  }
}

void ProjectHandler::UpdateRecentProjectNames()
{
  m_recent_projects->AddToRecentProjectList(
      QString::fromStdString(m_project_manager->GetProject()->GetPath()));
}

}  // namespace mvvm
