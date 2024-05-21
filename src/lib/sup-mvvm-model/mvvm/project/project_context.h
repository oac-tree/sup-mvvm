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
#include <vector>

namespace mvvm
{

class SessionModelInterface;

/**
 * @brief The ProjectContext class defines context necessary for Project construction.
 */
struct MVVM_MODEL_EXPORT ProjectContext
{
  //!< To notify about the change of the project with respect to what was written on disk.
  using modified_callback_t = std::function<void()>;

  //! To ask for a vector of models to save/load to/from disk.
  //! This is intentionally obtained via callback since save request might come after
  //! the Project construction.
  using models_callback_t = std::function<std::vector<SessionModelInterface*>()>;

  modified_callback_t modified_callback;
  models_callback_t models_callback;
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

#endif  // MVVM_PROJECT_PROJECT_TYPES_H_
