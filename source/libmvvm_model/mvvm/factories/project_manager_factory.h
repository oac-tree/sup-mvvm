/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_FACTORIES_PROJECTMANAGERFACTORY_H
#define MVVM_FACTORIES_PROJECTMANAGERFACTORY_H

#include "mvvm/interfaces/project_manager_interface.h"

#include <memory>

namespace mvvm
{

struct ProjectContext;
struct UserInteractionContext;

//! Creates default ProjectManager to save and load models.
MVVM_MODEL_EXPORT std::unique_ptr<ProjectManagerInterface> CreateProjectManager(
    const ProjectContext& project_context, const UserInteractionContext& user_context);

}  // namespace mvvm

#endif  // MVVM_FACTORIES_PROJECTMANAGERFACTORY_H
