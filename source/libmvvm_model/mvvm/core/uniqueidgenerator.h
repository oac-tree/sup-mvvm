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

#ifndef MVVM_CORE_UNIQUEIDGENERATOR_H
#define MVVM_CORE_UNIQUEIDGENERATOR_H

#include "mvvm/model_export.h"

#include <string>

namespace ModelView
{
//! Provides generation of unique SessionItem identifier.

//! In the future might be turned to singleton to keep track of all generated identifier
//! and make sure, that SessionItem identifiers loaded from disk, are different from those
//! generated during a dynamic session. For the moment though, we rely on zero-probability of
//! such event.

class MVVM_MODEL_EXPORT UniqueIdGenerator
{
public:
  static std::string Generate();
};

}  // namespace ModelView

#endif  // MVVM_CORE_UNIQUEIDGENERATOR_H
