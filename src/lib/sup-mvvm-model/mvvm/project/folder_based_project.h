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

#ifndef MVVM_PROJECT_FOLDER_BASED_PROJECT_H_
#define MVVM_PROJECT_FOLDER_BASED_PROJECT_H_

#include <mvvm/project/external_model_project.h>

namespace mvvm
{

struct ProjectContext;

/**
 * @brief The FolderBasedProject class represents content of all application models in a folder on
 * disk.
 *
 * The folder contains xml files, one file per model.
 */
class MVVM_MODEL_EXPORT FolderBasedProject : public ExternalModelProject
{
public:
  explicit FolderBasedProject(const std::vector<ISessionModel*>& models,
                              const ProjectContext& context);

private:
  bool SaveImpl(const std::string& path) override;
  bool LoadImpl(const std::string& path) override;
};

}  // namespace mvvm

#endif  // MVVM_PROJECT_FOLDER_BASED_PROJECT_H_
