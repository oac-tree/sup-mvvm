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

#include "project_manager_factory.h"

#include <mvvm/project/project_manager_decorator.h>

namespace mvvm
{
std::unique_ptr<ProjectManagerInterface> CreateProjectManager(
    const ProjectContext& project_context, const UserInteractionContext& user_context)
{
  return std::make_unique<ProjectManagerDecorator>(project_context, user_context);
}

}  // namespace mvvm
