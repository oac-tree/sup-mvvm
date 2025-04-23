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

#ifndef MVVM_PROJECT_PROJECT_MANAGER_FACTORY_H_
#define MVVM_PROJECT_PROJECT_MANAGER_FACTORY_H_

#include <mvvm/project/i_project_manager.h>

#include <memory>

namespace mvvm
{

struct UserInteractionContext;
class IProject;

/**
 * @brief Creates default project manager.
 *
 * @param project Project to manipulate.
 * @param user_context The context to interact with the user.
 * @return New project manager.
 */
MVVM_MODEL_EXPORT std::unique_ptr<IProjectManager> CreateProjectManager(
    IProject& project, const UserInteractionContext& user_context);

}  // namespace mvvm

#endif  // MVVM_PROJECT_PROJECT_MANAGER_FACTORY_H_
