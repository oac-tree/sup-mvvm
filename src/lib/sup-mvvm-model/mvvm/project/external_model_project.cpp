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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "external_model_project.h"

#include "project_context.h"

#include <mvvm/factories/model_document_factory.h>
#include <mvvm/model/i_session_model.h>

namespace mvvm
{

ExternalModelProject::ExternalModelProject(ProjectType project_type,
                                           const std::vector<ISessionModel*>& models,
                                           const ProjectContext& context)
    : AbstractProject(project_type, context), m_models(models)
{
  SetupListener(m_models);
}

std::vector<ISessionModel*> ExternalModelProject::GetModels() const
{
  return m_models;
}

bool ExternalModelProject::CloseProjectImpl()
{
  for (auto model : GetModels())
  {
    model->Clear();
  }

  return true;
}

bool ExternalModelProject::CreateEmptyProjectImpl()
{
  for (auto model : GetModels())
  {
    model->Clear();
  }

  return true;
}

}  // namespace mvvm
