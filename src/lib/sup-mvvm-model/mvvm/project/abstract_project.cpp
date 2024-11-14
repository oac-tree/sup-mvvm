/******************************************************************************
 *
 * FolderBasedProject       : Operational Applications UI Foundation
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

#include "abstract_project.h"

#include "project_change_controller.h"

#include <mvvm/core/exceptions.h>
#include <mvvm/model/i_session_model.h>

namespace mvvm
{

AbstractProject::AbstractProject(ProjectType project_type, const ProjectContext &context)
    : m_project_type(project_type), m_project_context(context)
{
}

AbstractProject::~AbstractProject() = default;

ProjectType AbstractProject::GetProjectType() const
{
  return m_project_type;
}

std::string AbstractProject::GetApplicationType() const
{
  return m_project_context.application_type;
}

std::string AbstractProject::GetPath() const
{
  return m_project_path;
}

bool AbstractProject::HasPath() const
{
  return !m_project_path.empty();
}

bool AbstractProject::IsModified() const
{
  return m_change_controller ? m_change_controller->IsChanged() : false;
}

bool AbstractProject::CreateEmpty()
{
  m_project_path.clear();
  m_change_controller.reset();

  auto is_success = CreateEmptyProjectImpl();
  if (is_success)
  {
    SetupListener(GetModels());
    ProjectLoadedNotify();
  }

  return is_success;
}

bool AbstractProject::Close()
{
  auto is_success = CloseProjectImpl();
  if (is_success)
  {
    m_project_path.clear();
    m_change_controller.reset();
  }

  return is_success;
}

bool AbstractProject::Save(const std::string &path)
{
  auto result = SaveImpl(path);
  if (result)
  {
    m_project_path = path;
    MarkProjectAsUnmodified();
  }
  return result;
}

bool AbstractProject::Load(const std::string &path)
{
  auto is_success = LoadImpl(path);
  if (is_success)
  {
    m_project_path = path;
    MarkProjectAsUnmodified();
    ProjectLoadedNotify();
  }
  return is_success;
}

std::vector<ISessionModel *> AbstractProject::GetModels() const
{
  return {};
}

void AbstractProject::SetupListener(const std::vector<ISessionModel *> &models)
{
  m_change_controller =
      std::make_unique<ProjectChangedController>(models, m_project_context.modified_callback);
}

void AbstractProject::ProjectLoadedNotify()
{
  if (m_project_context.loaded_callback)
  {
    m_project_context.loaded_callback();
  }
}

void AbstractProject::MarkProjectAsUnmodified()
{
  if (m_change_controller)
  {
    m_change_controller->ResetIsChanged();
  }
}

}  // namespace mvvm
