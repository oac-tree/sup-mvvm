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

#ifndef MVVM_INTERFACES_PROJECT_MANAGER_INTERFACE_H_
#define MVVM_INTERFACES_PROJECT_MANAGER_INTERFACE_H_

#include <mvvm/model_export.h>

#include <string>

namespace mvvm
{

//! Interface class for ProjectManager family.

//! Responsible for handling new/save/save-as/close Project logic, where the Project represents
//! a collection of serialized application models in the project directory.

class MVVM_MODEL_EXPORT ProjectManagerInterface
{
public:
  virtual ~ProjectManagerInterface() = default;
  virtual bool CreateNewProject(const std::string& dirname) = 0;

  virtual bool SaveCurrentProject() = 0;

  virtual bool SaveProjectAs(const std::string& dirname) = 0;

  virtual bool OpenExistingProject(const std::string& dirname) = 0;

  virtual std::string CurrentProjectDir() const = 0;

  virtual bool IsModified() const = 0;

  virtual bool CloseCurrentProject() const = 0;
};

}  // namespace mvvm

#endif  // MVVM_INTERFACES_PROJECT_MANAGER_INTERFACE_H_
