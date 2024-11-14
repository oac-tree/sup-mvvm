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

#ifndef MVVM_PROJECT_I_PROJECT_MANAGER_H_
#define MVVM_PROJECT_I_PROJECT_MANAGER_H_

#include <mvvm/model_export.h>
#include <mvvm/project/project_types.h>

#include <string>

namespace mvvm
{

class IProject;

/**
 * @brief The IProjectManager class is an interface for the ProjectManager family to save and load
 * projects.
 *
 * Responsible for handling new/save/save-as/close project logic. A project is represented by the
 * IProject class, which does actual save/load job.
 */
class MVVM_MODEL_EXPORT IProjectManager
{
public:
  virtual ~IProjectManager() = default;

  /**
   * @brief Returns true if project was modified since last save.
   */
  virtual bool IsModified() const = 0;

  /**
   * @brief Creates new project in a given path.
   *
   * For folder-based projects path is a full path to the project folder. For file-based projects,
   * it is a full path to a project file.
   *
   * @param path The full path where to create a new project.
   * @return True in the case of success.
   */
  virtual bool CreateNewProject(const std::string& path) = 0;

  /**
   * @brief Closes current project (without saving).
   *
   * @return Returns true in the case of success.
   *
   * @details No checks whether it is modified or not being performed.
   */
  virtual bool CloseProject() = 0;

  /**
   * @brief Saves current project.
   *
   * @return True in the case of success.
   */
  virtual bool SaveCurrentProject() = 0;

  /**
   * @brief Saves the project using a given path.
   *
   * For folder-based projects path is a full path to the project folder. For file-based projects,
   * it is a full path to a project file.
   *
   * @param path The full path to the project.
   * @return Returns true in the case of success.
   */
  virtual bool SaveProjectAs(const std::string& path) = 0;

  /**
   * @brief Opens existing project.
   *
   * @param dirname The full path to the project.
   * @return Returns true in the case of success.
   */
  virtual bool OpenExistingProject(const std::string& path) = 0;

  /**
   * @brief Returns underlying project.
   */
  virtual IProject* GetProject() const = 0;
};

}  // namespace mvvm

#endif  // MVVM_PROJECT_I_PROJECT_MANAGER_H_
