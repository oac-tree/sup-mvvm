// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#ifndef MVVM_INTERFACES_PROJECTMANAGERINTERFACE_H
#define MVVM_INTERFACES_PROJECTMANAGERINTERFACE_H

#include "mvvm/model_export.h"

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

#endif  // MVVM_INTERFACES_PROJECTMANAGERINTERFACE_H
