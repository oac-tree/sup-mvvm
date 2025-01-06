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

#include "recent_project_path.h"

#include <mvvm/utils/file_utils.h>

#include <QDir>

namespace mvvm
{

RecentProjectPath::RecentProjectPath(int max_project_count)
    : m_max_recent_project_count(max_project_count)
{
}

QString RecentProjectPath::GetCurrentWorkdir() const
{
  return m_current_workdir;
}

void RecentProjectPath::SetCurrentWorkdir(const QString& dir_name)
{
  m_current_workdir = dir_name;
}

void RecentProjectPath::UpdateWorkdirFromPath(const QString& project_path)
{
  if (!project_path.isEmpty())
  {
    // working directory is a parent of project path
    auto parent_path = utils::GetParentPath(project_path.toStdString());
    m_current_workdir = QString::fromStdString(parent_path);
  }
}

QStringList RecentProjectPath::GetRecentProjectList()
{
  ValidateIfProjectsExist();
  return m_recent_projects;
}

void RecentProjectPath::SetRecentProjectList(const QStringList& recent_project_list)
{
  m_recent_projects = recent_project_list;
}

void RecentProjectPath::AddToRecentProjectList(const QString& project_path)
{
  m_recent_projects.removeAll(project_path);
  m_recent_projects.prepend(project_path);
  while (m_recent_projects.size() > m_max_recent_project_count)
  {
    m_recent_projects.removeLast();
  }
}

void RecentProjectPath::ClearRecentProjectsList()
{
  m_recent_projects.clear();
}

void RecentProjectPath::ValidateIfProjectsExist()
{
  QStringList updated_list;
  for (const auto& file_name : m_recent_projects)
  {
    if (utils::IsExists(file_name.toStdString()))
    {
      updated_list.append(file_name);
    }
  }
  m_recent_projects = updated_list;
}

}  // namespace mvvm
