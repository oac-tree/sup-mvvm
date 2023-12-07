/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "project.h"

#include "project_change_controller.h"
#include "project_types.h"
#include "project_utils.h"

#include <mvvm/factories/model_document_factory.h>
#include <mvvm/utils/file_utils.h>

#include <functional>

namespace mvvm
{

struct Project::ProjectImpl
{
  std::string m_project_dir;
  ProjectContext m_context;
  ProjectChangedController m_change_controller;

  explicit ProjectImpl(const ProjectContext& context)
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

Project::Project(const ProjectContext& context) : p_impl(std::make_unique<ProjectImpl>(context)) {}

Project::~Project() = default;

//! Returns the full path to a project directory. It is a name where the project has been last time
//! saved, or loaded from.

std::string Project::GetProjectDir() const
{
  return p_impl->m_project_dir;
}

//! Saves all models to a given directory. Directory should exist.
//! Provided name will become 'projectDir'.

bool Project::Save(const std::string& dirname) const
{
  return p_impl->Process(dirname, &ModelDocumentInterface::Save);
}

//! Loads all models from the given directory.
bool Project::Load(const std::string& dirname)
{
  return p_impl->Process(dirname, &ModelDocumentInterface::Load);
}

bool Project::IsModified() const
{
  return p_impl->m_change_controller.IsChanged();
}

}  // namespace mvvm
