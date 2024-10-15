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

#include "file_based_project.h"

#include "project_context.h"

#include <mvvm/factories/model_document_factory.h>
#include <mvvm/model/i_session_model.h>

namespace mvvm
{

FileBasedProject::FileBasedProject(const std::vector<ISessionModel *> &models,
                                   const ProjectContext &context)
    : AbstractProject(ProjectType::kFileBased, context), m_models(models)
{
  SetupListener(m_models);
}

std::vector<ISessionModel *> FileBasedProject::GetModels() const
{
  return m_models;
}

bool FileBasedProject::SaveImpl(const std::string &path)
{
  auto document = CreateXmlDocument(GetModels(), GetApplicationType());
  document->Save(path);
  // underlying XmlDocument reports errors as exceptions
  return true;
}

bool FileBasedProject::LoadImpl(const std::string &path)
{
  auto document = CreateXmlDocument(GetModels(), GetApplicationType());
  document->Load(path);
  // underlying XmlDocument reports errors as exceptions
  return true;
}

bool FileBasedProject::CloseProjectImpl()
{
  for (auto model : GetModels())
  {
    model->Clear();
  }

  return true;
}

bool FileBasedProject::CreateNewProjectImpl()
{
  for (auto model : GetModels())
  {
    model->Clear();
  }

  return true;
}

}  // namespace mvvm
