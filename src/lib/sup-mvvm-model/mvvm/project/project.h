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

#ifndef MVVM_PROJECT_PROJECT_H_
#define MVVM_PROJECT_PROJECT_H_

#include <mvvm/interfaces/project_interface.h>

#include <memory>

namespace mvvm
{

struct ProjectContext;

//! Project represents content of all application models in a folder on disk.
//! Responsible for saving/loading application models to/from disk.

class MVVM_MODEL_EXPORT Project : public ProjectInterface
{
public:
  explicit Project(const ProjectContext& context);
  ~Project() override;

  std::string GetProjectDir() const override;

  bool Save(const std::string& dirname) const override;

  bool Load(const std::string& dirname) override;

  bool IsModified() const override;

private:
  struct ProjectImpl;
  std::unique_ptr<ProjectImpl> p_impl;
};

}  // namespace mvvm

#endif  // MVVM_PROJECT_PROJECT_H_
