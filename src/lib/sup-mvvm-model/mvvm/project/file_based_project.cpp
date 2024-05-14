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

#include "project_change_controller.h"
#include "project_context.h"
#include "project_utils.h"

#include <mvvm/factories/model_document_factory.h>
#include <mvvm/utils/file_utils.h>

#include <functional>

namespace mvvm
{

struct FileBasedProject::FileBasedProjectImpl
{
  std::string m_project_path;
  ProjectContext m_context;
  ProjectChangedController m_change_controller;

  explicit FileBasedProjectImpl(const ProjectContext& context)
      : m_context(context)
      , m_change_controller(context.m_models_callback(), context.m_modified_callback)
  {
  }

  //! Returns list of models which are subject to save/load.
  std::vector<SessionModelInterface*> GetModels() const { return m_context.m_models_callback(); }

  //! Processes all models one by one and either save or load them to/from given directory.
  //! Template parameter `method` specifies ModelDocumentInterface's method to use.
  template <typename T>
  bool Process(const std::string& path, T method)
  {
    auto document = CreateXmlDocument(GetModels());
    std::invoke(method, document, path);

    m_project_path = path;
    m_change_controller.ResetIsChanged();
    return true;
  }
};

FileBasedProject::FileBasedProject(const ProjectContext& context)
    : p_impl(std::make_unique<FileBasedProjectImpl>(context))
{
}

ProjectType FileBasedProject::GetProjectType() const
{
  return ProjectType::kFileBased;
}

FileBasedProject::~FileBasedProject() = default;

std::string FileBasedProject::GetProjectPath() const
{
  return p_impl->m_project_path;
}

bool FileBasedProject::Save(const std::string& path)
{
  return p_impl->Process(path, &IModelDocument::Save);
}

bool FileBasedProject::Load(const std::string& path)
{
  return p_impl->Process(path, &IModelDocument::Load);
}

bool FileBasedProject::IsModified() const
{
  return p_impl->m_change_controller.IsChanged();
}

}  // namespace mvvm
