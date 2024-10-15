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

#ifndef MVVM_PROJECT_PROJECT_CONTEXT_H_
#define MVVM_PROJECT_PROJECT_CONTEXT_H_

#include <mvvm/model_export.h>
#include <mvvm/project/project_types.h>

#include <functional>
#include <string>

namespace mvvm
{

class ISessionModel;

/**
 * @brief The ProjectContext class defines context necessary for Project construction.
 */
struct MVVM_MODEL_EXPORT ProjectContext
{
  //!< notifies about the change of the project with respect to what was written on disk
  std::function<void()> modified_callback;

  //!< notifies at the end of successfull project load (loaded from disk, re-created as new)
  std::function<void()> loaded_callback;

  //!< Application type allows to distinguish model documents created by various MVVM applications.
  //! The name is used during document save as an attribute of root XML element, and is validated on
  //! document load.
  std::string application_type;
};

/**
 * @brief The UserInteractionContext class defines the context to interact with the user regarding
 * save/save-as/create-new project scenario.
 *
 * Callbacks returning paths should return full path to a project file (for file-based projects),
 * or full path to a project folder (for folder-based projects).
 */
struct MVVM_MODEL_EXPORT UserInteractionContext
{
  std::function<std::string()> existing_path_callback;  //!< to ask existing project path
  std::function<std::string()> new_path_callback;       //!< to ask for new path
  std::function<SaveChangesAnswer()> answer_callback;   //!< to ask what to do with modified project
  std::function<void(std::string)> message_callback;    //!< to report exception message to the user
};

}  // namespace mvvm

#endif  // MVVM_PROJECT_PROJECT_CONTEXT_H_
