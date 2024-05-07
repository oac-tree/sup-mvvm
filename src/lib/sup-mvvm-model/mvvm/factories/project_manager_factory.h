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

#ifndef MVVM_FACTORIES_PROJECT_MANAGER_FACTORY_H_
#define MVVM_FACTORIES_PROJECT_MANAGER_FACTORY_H_

#include <mvvm/project/i_project_manager.h>

#include <functional>
#include <memory>

namespace mvvm
{

struct UserInteractionContext;
class IProject;

/**
 * @brief Creates default project manager.
 *
 * @param project_factory_func Factory function to create projects.
 * @param user_context The context to interact with the user.
 * @return New project manager.
 */
MVVM_MODEL_EXPORT std::unique_ptr<IProjectManager> CreateProjectManager(
    std::function<std::unique_ptr<IProject>()> project_factory_func,
    const UserInteractionContext& user_context);

}  // namespace mvvm

#endif  // MVVM_FACTORIES_PROJECT_MANAGER_FACTORY_H_
