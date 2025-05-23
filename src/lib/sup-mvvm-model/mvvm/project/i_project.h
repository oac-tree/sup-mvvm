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

#ifndef MVVM_PROJECT_I_PROJECT_H_
#define MVVM_PROJECT_I_PROJECT_H_

#include <mvvm/model_export.h>
#include <mvvm/project/project_types.h>

#include <string>

namespace mvvm
{

/**
 * @brief The IProject class is an interface to save and load application state.
 *
 * Conceptually, a project is a serialized content of all application models. Depending on the type
 * of the project, it can be either a single file on disk, or a folder on disk with several files in
 * it.
 *
 * The IProject-derived classes contain the logic on how to save and load the data from certain
 * place. They shouldn't contain any logic related to unsaved data. The latter is handled by
 * IProjectManager.
 */
class MVVM_MODEL_EXPORT IProject
{
public:
  virtual ~IProject() = default;

  /**
   * @brief Returns the type of the project.
   */
  virtual ProjectType GetProjectType() const = 0;

  /**
   * @brief Returns the type of the application.
   *
   * The value is used to determine if the document can be opened by the application.
   */
  virtual std::string GetApplicationType() const = 0;

  /**
   * @brief Returns the full path to a project.
   *
   * For folder-based projects it is a full path to the project folder. For file-based projects, it
   * is a full path to a project file. The path can be empty if a project was just created and never
   * saved.
   *
   * @return The project path where the project has been last time saved or loaded from.
   */
  virtual std::string GetPath() const = 0;

  /**
   * @brief Checks if project has correct path defined.
   *
   * Meaning it was already correctly saved to, or loaded from certain place.
   */
  virtual bool HasPath() const = 0;

  /**
   * @brief Checks if the project has been modified since the last save.
   */
  virtual bool IsModified() const = 0;

  /**
   * @brief Creates empty in-memory project.
   *
   * Depending on the implementation, it recreates application models, or cleans-up existing models
   * from content.
   *
   * The method doesn't perform any checks for unsaved data, and it doesn't save empty project on
   * disk after creation.
   */
  virtual bool CreateEmpty() = 0;

  /**
   * @brief Close the project.
   *
   * Depending on the implementation, it can clean-up underlying models from content, or destroy
   * models completely.
   *
   * The method doesn't perform any checks for unsaved data.
   */
  virtual bool Close() = 0;

  /**
   * @brief Saves the project content to a given path.
   *
   * Given path will become new project path as reported by GetProjectPath() method.
   *
   * @param path The full path where to save.
   * @return True in the case of success.
   */
  virtual bool Save(const std::string& path) = 0;

  /**
   * @brief Loads project content from the given path.
   *
   * Given path will become new project path as reported by GetProjectPath() method.
   *
   * @return True in the case of success.
   */
  virtual bool Load(const std::string& path) = 0;
};

}  // namespace mvvm

#endif  // MVVM_PROJECT_I_PROJECT_H_
