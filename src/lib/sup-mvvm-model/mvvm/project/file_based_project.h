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

#ifndef MVVM_PROJECT_FILE_BASED_PROJECT_H_
#define MVVM_PROJECT_FILE_BASED_PROJECT_H_

#include <mvvm/project/i_project.h>

#include <memory>

namespace mvvm
{

struct ProjectContext;

/**
 * @brief The FileBasedProject class represents content of all application models in a folder on
 * disk.
 */
class MVVM_MODEL_EXPORT FileBasedProject : public IProject
{
public:
  explicit FileBasedProject(const ProjectContext& context);
  ~FileBasedProject() override;

  ProjectType GetProjectType() const override;

  std::string GetProjectPath() const override;

  /**
   * @details The directory should exist to succeed.
   */
  bool Save(const std::string& path) const override;

  bool Load(const std::string& path) override;

  bool IsModified() const override;

private:
  struct FileBasedProjectImpl;
  std::unique_ptr<FileBasedProjectImpl> p_impl;
};

}  // namespace mvvm

#endif  // MVVM_PROJECT_FILE_BASED_PROJECT_H_
