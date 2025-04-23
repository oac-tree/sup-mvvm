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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
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
    : ExternalModelProject(ProjectType::kFileBased, models, context)
{
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

}  // namespace mvvm
