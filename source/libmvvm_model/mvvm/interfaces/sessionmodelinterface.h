/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_INTERFACES_SESSIONMODELINTERFACE_H
#define MVVM_INTERFACES_SESSIONMODELINTERFACE_H

#include "mvvm/model/function_types.h"
#include "mvvm/model_export.h"

namespace ModelView
{
//! Application model interface.

class MVVM_MODEL_EXPORT SessionModelInterface
{
public:
  virtual ~SessionModelInterface() = default;
};

}  // namespace ModelView

#endif  // MVVM_INTERFACES_SESSIONMODELINTERFACE_H
