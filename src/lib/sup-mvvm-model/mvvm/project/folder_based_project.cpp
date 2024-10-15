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

#include "folder_based_project.h"

#include "project_context.h"
#include "project_utils.h"

#include <mvvm/factories/model_document_factory.h>
#include <mvvm/utils/file_utils.h>

namespace
{

/**
 * @brief Processes all models one by one and either save or load them to/from given directory.
 *
 * @param dirname Full path to the directory to save the models.
 * @tparam method One of two methods Save/Load of ModelDocumentInterface.
 * @param models
 *
 * @return True in the case of success, otherwise false.
 */
template <typename T>
bool Process(const std::string& dirname, T method, std::vector<mvvm::ISessionModel*> models,
             const std::string& application_type)
{
  if (!mvvm::utils::IsExists(dirname))
  {
    return false;
  }

  for (auto model : models)
  {
    // single xml file per model
    auto document = mvvm::CreateXmlDocument({model}, application_type);
    auto filename = mvvm::utils::Join(dirname, mvvm::utils::SuggestFileName(*model));
    std::invoke(method, document, filename);
  }

  return true;
}

}  // namespace

namespace mvvm
{

FolderBasedProject::FolderBasedProject(const ProjectContext& context)
    : AbstractProject(ProjectType::kFolderBased, context)
{
}

bool FolderBasedProject::SaveImpl(const std::string& path)
{
  return Process(path, &IModelDocument::Save, GetModels(), GetApplicationType());
}

bool FolderBasedProject::LoadImpl(const std::string& path)
{
  return Process(path, &IModelDocument::Load, GetModels(), GetApplicationType());
}

bool FolderBasedProject::CloseProjectImpl()
{
  return true;
}

bool FolderBasedProject::CreateNewProjectImpl()
{
  return true;
}

}  // namespace mvvm
