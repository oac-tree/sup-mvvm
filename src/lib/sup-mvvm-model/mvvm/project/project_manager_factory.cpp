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

#include "project_manager_factory.h"

#include <mvvm/project/project_manager.h>

namespace mvvm
{

std::unique_ptr<IProjectManager> CreateProjectManager(IProject& project,
                                                      const UserInteractionContext& user_context)
{
  // creating decoration for user interaction
  return std::make_unique<ProjectManager>(&project, user_context);
}

}  // namespace mvvm
