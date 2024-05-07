/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
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

#include "project_manager_factory.h"

#include <mvvm/project/project_manager.h>
#include <mvvm/project/project_manager_decorator.h>

namespace mvvm
{
std::unique_ptr<IProjectManager> CreateProjectManager(
    std::function<std::unique_ptr<IProject>()> project_factory_func,
    const UserInteractionContext& user_context)
{
  // creating ProjectManager with basic save/save-as/open functionality
  auto project_manager = std::make_unique<ProjectManager>(project_factory_func);

  // creating decoration for user interaction
  return std::make_unique<ProjectManagerDecorator>(std::move(project_manager), user_context);
}

}  // namespace mvvm
