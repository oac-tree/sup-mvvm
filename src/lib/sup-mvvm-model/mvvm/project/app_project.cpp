/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
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

#include "app_project.h"

#include <mvvm/factories/model_document_factory.h>
#include <mvvm/model/i_session_model.h>
#include <mvvm/serialization/xml_document_helper.h>
#include <mvvm/utils/container_utils.h>

namespace mvvm
{

AppProject::AppProject(const mvvm::ProjectContext &context)
    : AbstractProject(mvvm::ProjectType::kFileBased, context)
{
}

std::vector<mvvm::ISessionModel *> AppProject::GetModels() const
{
  return mvvm::utils::GetVectorOfPtrs(m_models);
}

size_t AppProject::RegisterModel(model_factory_func_t factory_func)
{
  const size_t model_index = m_factory_func_list.size();
  m_factory_func_list.push_back(factory_func);
  return model_index;
}

size_t AppProject::GetModelCount() const
{
  return m_models.size();
}

bool AppProject::SaveImpl(const std::string &path)
{
  if (GetModels().empty())
  {
    throw RuntimeException("Attempt to save unexisting project");
  }

  auto document = mvvm::CreateXmlDocument(GetModels(), GetApplicationType());
  document->Save(path);

  return true;
}

bool AppProject::LoadImpl(const std::string &path)
{
  // creating and populating copies first
  auto models = CreateModels();
  auto document =
      mvvm::CreateXmlDocument(mvvm::utils::GetVectorOfPtrs(models), GetApplicationType());
  document->Load(path);

  // replacing old models
  m_models = std::move(models);

  return true;
}

bool AppProject::CloseProjectImpl()
{
  m_models.clear();
  return true;
}

bool AppProject::CreateNewProjectImpl()
{
  if (m_factory_func_list.empty())
  {
    return false;
  }

  m_models = CreateModels();
  return true;
}

std::vector<std::unique_ptr<mvvm::ISessionModel>> AppProject::CreateModels()
{
  std::vector<std::unique_ptr<mvvm::ISessionModel>> result;
  result.reserve(m_factory_func_list.size());
  for (const auto &factory_func : m_factory_func_list)
  {
    result.push_back(factory_func());
  }
  return result;
}

}  // namespace mvvm
