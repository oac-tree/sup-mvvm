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

#include "mock_project.h"

#include <mvvm/core/exceptions.h>

namespace mvvm::test
{

ProjectDecorator::ProjectDecorator(IProject *project) : m_project(project)
{
  if (!m_project)
  {
    throw RuntimeException("Uninitialised project");
  }
}

ProjectType ProjectDecorator::GetProjectType() const
{
  return m_project->GetProjectType();
}

std::string ProjectDecorator::GetApplicationType() const
{
  return m_project->GetApplicationType();
}

std::string ProjectDecorator::GetProjectPath() const
{
  return m_project->GetProjectPath();
}

bool ProjectDecorator::Save(const std::string &path)
{
  return m_project->Save(path);
}

bool ProjectDecorator::Load(const std::string &path)
{
  return m_project->Load(path);
}

bool ProjectDecorator::IsModified() const
{
  return m_project->IsModified();
}

}  // namespace mvvm::test
