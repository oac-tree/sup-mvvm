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

#ifndef MVVM_PROJECT_PROJECT_TYPES_H
#define MVVM_PROJECT_PROJECT_TYPES_H

#include "mvvm/model_export.h"

#include <functional>
#include <string>
#include <vector>

namespace mvvm
{

class ApplicationModel;

//! Possible user answers on question "Project was modified".
enum class SaveChangesAnswer
{
  kSave = 0,     //!< save project
  kDiscard = 1,  //!< discard changes
  kCancel = 2    //!< cancel current action, leave everything unsaved
};

//! Provides necessary information for Project construction.

struct MVVM_MODEL_EXPORT ProjectContext
{
  //!< To notify about the change of the project with respect to what was written on disk.
  using modified_callback_t = std::function<void()>;

  //! To ask for a vector of models to save/load to/from disk.
  //! This is intentionally obtained via callback since save request might come after
  //! the Project construction.
  using models_callback_t = std::function<std::vector<ApplicationModel*>()>;

  modified_callback_t m_modified_callback;
  models_callback_t m_models_callback;
};

//! Defines the context to interact with the user regarding save/save-as/create-new project
//! scenarious.

struct MVVM_MODEL_EXPORT UserInteractionContext
{
  //!< To ask the user to select existing directory, returns full path to the directory.
  using select_dir_callback_t = std::function<std::string()>;

  //!< To ask the user to create a new directory, returns full path to the directory.
  using create_dir_callback_t = std::function<std::string()>;

  //!< To ask the user what to do with modified project.
  using answer_callback_t = std::function<SaveChangesAnswer()>;

  select_dir_callback_t m_select_dir_callback;
  create_dir_callback_t m_create_dir_callback;
  answer_callback_t m_answer_callback;
};

}  // namespace mvvm

#endif  // MVVM_PROJECT_PROJECT_TYPES_H
