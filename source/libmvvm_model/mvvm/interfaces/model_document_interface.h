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

#ifndef MVVM_INTERFACES_MODELDOCUMENTINTERFACE_H
#define MVVM_INTERFACES_MODELDOCUMENTINTERFACE_H

#include "mvvm/model_export.h"

#include <string>

namespace mvvm
{

//! Pure virtual interface to save and restore models to/from disk.

class MVVM_MODEL_EXPORT ModelDocumentInterface
{
public:
  virtual ~ModelDocumentInterface() = default;

  virtual void Save(const std::string& file_name) const = 0;
  virtual void Load(const std::string& file_name) = 0;
};

}  // namespace mvvm

#endif  // MVVM_INTERFACES_MODELDOCUMENTINTERFACE_H
