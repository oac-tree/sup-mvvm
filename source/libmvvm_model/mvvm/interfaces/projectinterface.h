/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_INTERFACES_PROJECTINTERFACE_H
#define MVVM_INTERFACES_PROJECTINTERFACE_H

#include "mvvm/model_export.h"

#include <string>

namespace mvvm
{

//! Interface to manipulate projects on disk.
//! Project represents content of all application models in a folder on disk.

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
