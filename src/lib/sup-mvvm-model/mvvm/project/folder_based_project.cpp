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

#include "project_change_controller.h"
#include "project_context.h"
#include "project_utils.h"

#include <mvvm/factories/model_document_factory.h>
#include <mvvm/utils/file_utils.h>

#include <functional>

namespace mvvm
{

struct FolderBasedProject::FolderBasedProjectImpl
{
  std::string m_project_dir;
  ProjectContext m_context;
  ProjectChangedController m_change_controller;

  explicit FolderBasedProjectImpl(const ProjectContext& context)
      : m_context(context)
      , m_change_controller(context.m_models_callback(), context.m_modified_callback)
  {
  }

  //! Returns list of models which are subject to save/load.
  std::vector<SessionModelInterface*> GetModels() const { return m_context.m_models_callback(); }

  //! Processes all models one by one and either save or load them to/from given directory.
  //! Template parameter `method` specifies ModelDocumentInterface's method to use.
  template <typename T>
  bool Process(const std::string& dirname, T method)
  {
    if (!utils::IsExists(dirname))
    {
      return false;
    }

    for (auto model : GetModels())
    {
      auto document = CreateXmlDocument({model});
      auto filename = utils::Join(dirname, utils::SuggestFileName(*model));
      std::invoke(method, document, filename);
    }
    m_project_dir = dirname;
    m_change_controller.ResetIsChanged();
    return true;
  }
};

FolderBasedProject::FolderBasedProject(const ProjectContext& context)
    : p_impl(std::make_unique<FolderBasedProjectImpl>(context))
{
}

ProjectType FolderBasedProject::GetProjectType() const
{
  return ProjectType::kFolderBased;
}

FolderBasedProject::~FolderBasedProject() = default;

std::string FolderBasedProject::GetProjectDir() const
{
  return p_impl->m_project_dir;
}

bool FolderBasedProject::Save(const std::string& dirname) const
{
  return p_impl->Process(dirname, &IModelDocument::Save);
}

bool FolderBasedProject::Load(const std::string& dirname)
{
  return p_impl->Process(dirname, &IModelDocument::Load);
}

bool FolderBasedProject::IsModified() const
{
  return p_impl->m_change_controller.IsChanged();
}

}  // namespace mvvm
