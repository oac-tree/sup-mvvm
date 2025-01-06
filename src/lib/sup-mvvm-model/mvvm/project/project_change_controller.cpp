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

#include "project_change_controller.h"

#include <mvvm/project/model_has_changed_controller.h>

namespace mvvm
{

struct ProjectChangedController::ProjectChangedControllerImpl
{
  std::vector<ISessionModel*> m_models;
  std::vector<std::unique_ptr<ModelHasChangedController>> m_change_controllers;
  callback_t m_project_changed_callback;
  bool m_project_has_changed{false};

  ProjectChangedControllerImpl(const std::vector<ISessionModel*>& models,
                               callback_t callback)
      : m_models(models), m_project_changed_callback(std::move(callback))
  {
    CreateControllers();
  }

  void CreateControllers()
  {
    auto on_model_changed = [this]() { OnProjectHasChanged(); };
    m_change_controllers.clear();
    for (auto model : m_models)
    {
      m_change_controllers.emplace_back(
          std::make_unique<ModelHasChangedController>(model, on_model_changed));
    }
  }

  bool IsChanged() const { return m_project_has_changed; }

  void ResetIsChanged()
  {
    for (auto& controller : m_change_controllers)
    {
      controller->ResetIsChanged();
    }
    m_project_has_changed = false;
  }

  void OnProjectHasChanged()
  {
    if (!m_project_has_changed)
    {
      m_project_has_changed = true;
      if (m_project_changed_callback)
      {
        m_project_changed_callback();
      }
    }
  }
};

ProjectChangedController::ProjectChangedController(
    const std::vector<ISessionModel*>& models, callback_t project_changed_callback)
    : p_impl(
        std::make_unique<ProjectChangedControllerImpl>(models, std::move(project_changed_callback)))
{
}

ProjectChangedController::~ProjectChangedController() = default;

bool ProjectChangedController::IsChanged() const
{
  return p_impl->IsChanged();
}

void ProjectChangedController::ResetIsChanged()
{
  p_impl->ResetIsChanged();
}

}  // namespace mvvm
