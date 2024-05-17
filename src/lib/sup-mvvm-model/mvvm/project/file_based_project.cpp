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

namespace mvvm
{

FileBasedProject::FileBasedProject(const ProjectContext &context)
    : AbstractProject(ProjectType::kFileBased, context.m_models_callback(),
                      context.m_modified_callback)
{
}

bool FileBasedProject::SaveImpl(const std::string &path)
{
  auto document = CreateXmlDocument(GetModels(), GetProjectName());
  document->Save(path);
  return true;
}

bool FileBasedProject::LoadImpl(const std::string &path)
{
  auto document = CreateXmlDocument(GetModels(), GetProjectName());
  document->Load(path);
  return true;
}

}  // namespace mvvm
