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

#ifndef MVVM_PROJECT_I_PROJECT_USER_INTERACTOR_H_
#define MVVM_PROJECT_I_PROJECT_USER_INTERACTOR_H_

#include <mvvm/project/project_types.h>

#include <string>

namespace mvvm
{

/**
 * @brief The IProjectUserInteractor class defines an interface for the interaction with the user
 * while saving/loading projects.
 *
 * It allows one to get the path to create new projects, open existing ones, and get answers to the
 * question "The project was modified, do you want to save it?".
 */
class IProjectUserInteractor
{
public:
  virtual ~IProjectUserInteractor() = default;

  /**
   * @brief Returns a path to a new project to create.
   *
   * Depending on the implementation, it can be either an empty folder on disk, or a new file name.
   * Method is non-const since its implementation might update internal state (i.e. last used
   * working directory).
   *
   * @return Full project path.
   */
  virtual std::string GetNewProjectPath() = 0;

  /**
   * @brief Returns a path to existing project.
   *
   * Depending on the implementation, it can be either an existing folder on disk, or existing file.
   * Method is non-const since its implementation might update internal state (i.e. last used
   * working directory).
   *
   * @return Full project path.
   */
  virtual std::string GetExistingProjectPath() = 0;

  /**
   * @brief Returns save/cancel/discard changes choice provided by the user.
   */
  virtual mvvm::SaveChangesAnswer OnSaveCurrentChangesRequest() const = 0;

  /**
   * @brief Sends message to the user.
   */
  virtual void SendMessage(const std::string& message) const = 0;
};

}  // namespace mvvm

#endif  // MVVM_PROJECT_I_PROJECT_USER_INTERACTOR_H_
