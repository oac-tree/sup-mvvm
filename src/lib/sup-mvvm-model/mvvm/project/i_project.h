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

#ifndef MVVM_PROJECT_IPROJECT_H_
#define MVVM_PROJECT_IPROJECT_H_

#include <mvvm/model_export.h>
#include <mvvm/project/project_types.h>

#include <string>

namespace mvvm
{

/**
 * @brief The IProject class is an interface to save and load application state.
 *
 * Conceptually, a project is a serialized content of all application models. Depending on the type
 * of the project, it can be either a single file or a folder on disk with several files in it.
 *
 * The IProject-derived classes contain the logic on how to save and load the data from certain
 * place. The logic of directory creation, rewriting existing projects, discarding unsaved project
 * etc is handled by the IProjectManager.
 */
class MVVM_MODEL_EXPORT IProject
{
public:
  virtual ~IProject() = default;

  /**
   * @brief Returns type of the project.
   */
  virtual ProjectType GetProjectType() const = 0;

  /**
   * @brief Returns the full path to a project directory.
   *
   * @details It is a name where the project has been last time saved, or loaded from.
   */
  virtual std::string GetProjectDir() const = 0;

  /**
   * @brief Saves the content to a given directory.
   * @param dirname The full path to existing directory on disk.
   * @return True in the case of success.
   *
   * @details Provided directory name will become new project directory as reported by
   * GetProjectDir() method.
   */
  virtual bool Save(const std::string& dirname) const = 0;

  /**
   * @brief Loads all content from the given directory.
   * @return True in the case of success.
   */
  virtual bool Load(const std::string& dirname) = 0;

  /**
   * @brief Returns true if the project has been modified since the last save.
   */
  virtual bool IsModified() const = 0;
};

}  // namespace mvvm

#endif  // MVVM_PROJECT_IPROJECT_H_
