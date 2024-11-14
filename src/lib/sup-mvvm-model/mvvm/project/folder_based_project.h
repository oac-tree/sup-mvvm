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

#ifndef MVVM_PROJECT_FOLDER_BASED_PROJECT_H_
#define MVVM_PROJECT_FOLDER_BASED_PROJECT_H_

#include <mvvm/project/abstract_project.h>

namespace mvvm
{

struct ProjectContext;

/**
 * @brief The FolderBasedProject class represents content of all application models in a folder on
 * disk.
 *
 * The folder contains xml files, one file per model.
 */
class MVVM_MODEL_EXPORT FolderBasedProject : public AbstractProject
{
public:
  explicit FolderBasedProject(const std::vector<ISessionModel*>& models,
                              const ProjectContext& context);

  std::vector<ISessionModel*> GetModels() const override;

private:
  bool SaveImpl(const std::string& path) override;
  bool LoadImpl(const std::string& path) override;
  bool CloseProjectImpl() override;
  bool CreateEmptyProjectImpl() override;

  std::vector<ISessionModel*> m_models;
};

}  // namespace mvvm

#endif  // MVVM_PROJECT_FOLDER_BASED_PROJECT_H_
