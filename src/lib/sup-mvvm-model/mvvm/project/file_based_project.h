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

#ifndef MVVM_PROJECT_FILE_BASED_PROJECT_H_
#define MVVM_PROJECT_FILE_BASED_PROJECT_H_

#include <mvvm/project/external_model_project.h>

namespace mvvm
{

struct ProjectContext;

/**
 * @brief The FileBasedProject class represents content of several application models in a single
 * XML file on disk.
 */
class MVVM_MODEL_EXPORT FileBasedProject : public ExternalModelProject
{
public:
  explicit FileBasedProject(const std::vector<ISessionModel*>& models,
                            const ProjectContext& context);

private:
  bool SaveImpl(const std::string& path) override;
  bool LoadImpl(const std::string& path) override;
};

}  // namespace mvvm

#endif  // MVVM_PROJECT_FILE_BASED_PROJECT_H_
