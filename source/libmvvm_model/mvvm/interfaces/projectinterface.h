// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#ifndef MVVM_INTERFACES_PROJECTINTERFACE_H
#define MVVM_INTERFACES_PROJECTINTERFACE_H

#include "mvvm/model_export.h"

#include <string>

namespace mvvm
{

//! Interface to manipulate projects on disk.
//! The project represents a directory with a collection of files, representing content of all
//! application models in a folder on disk.

class MVVM_MODEL_EXPORT ProjectInterface
{
public:
  virtual ~ProjectInterface() = default;
  virtual std::string GetProjectDir() const = 0;
  virtual bool Save(const std::string& dirname) const = 0;
  virtual bool Load(const std::string& dirname) = 0;
  virtual bool IsModified() const = 0;
};

}  // namespace mvvm

#endif  // MVVM_INTERFACES_PROJECTINTERFACE_H
