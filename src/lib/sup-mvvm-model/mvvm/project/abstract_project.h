/******************************************************************************
 *
 * FolderBasedProject       : Operational Applications UI Foundation
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

#ifndef MVVM_PROJECT_ABSTRACT_PROJECT_H_
#define MVVM_PROJECT_ABSTRACT_PROJECT_H_

#include <mvvm/project/i_project.h>

#include <functional>
#include <memory>

namespace mvvm
{

class ProjectChangedController;
class SessionModelInterface;

/**
 * @brief The AbstractProject class implements common functionality for file-based and folder-based
 * projects.
 *
 * It takes care of notification when project is changed, and traces project path.
 */
class MVVM_MODEL_EXPORT AbstractProject : public IProject
{
public:
  using modified_callback_t = std::function<void()>;

  /**
   * @brief Main c-tor.
   *
   * @param project_type Type of the project.
   * @param models List of models participating in save/load.
   * @param callback A callback to report notifications.
   * @param project_name The name of the project to form XML document.
   */
  AbstractProject(ProjectType project_type, const std::vector<SessionModelInterface*>& models,
                  modified_callback_t callback, const std::string& project_name = {});
  ~AbstractProject() override;

  ProjectType GetProjectType() const override;

  std::string GetProjectName() const override;

  std::string GetProjectPath() const override;

  bool Save(const std::string& path) override;

  bool Load(const std::string& path) override;

  bool IsModified() const override;

protected:
  std::vector<SessionModelInterface*> GetModels() const;

private:
  virtual bool SaveImpl(const std::string&) = 0;
  virtual bool LoadImpl(const std::string&) = 0;

  std::string m_project_path;
  ProjectType m_project_type;
  std::vector<SessionModelInterface*> m_models;
  std::unique_ptr<ProjectChangedController> m_change_controller;
  std::string m_project_name;
};

}  // namespace mvvm

#endif  // MVVM_PROJECT_ABSTRACT_PROJECT_H_
