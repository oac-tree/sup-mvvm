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

#ifndef MVVM_PROJECT_PROJECT_TYPES_H_
#define MVVM_PROJECT_PROJECT_TYPES_H_

#include <mvvm/model_export.h>

#include <cstdint>

namespace mvvm
{

/**
 * @brief The ProjectType enum defines possible types of the project.
 *
 * The project is a class derived from IProject, representing a serialized content of the model.
 */
enum class ProjectType : std::uint8_t
{
  kFolderBased,  //!< project is a folder on disk with some files in it
  kFileBased     //!< project is a single file
};

/**
 * @brief The SaveChangesAnswer enum represents user answers to the question "Project was modified,
 * do you want to save it?".
 */
enum class SaveChangesAnswer : std::uint8_t
{
  kSave = 0,     //!< save project
  kDiscard = 1,  //!< discard changes
  kCancel = 2    //!< cancel current action, leave everything unsaved
};

}  // namespace mvvm

#endif  // MVVM_PROJECT_PROJECT_TYPES_H_
