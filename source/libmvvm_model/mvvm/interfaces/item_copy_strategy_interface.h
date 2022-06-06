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

#ifndef MVVM_INTERFACES_ITEMCOPYSTRATEGYINTERFACE_H
#define MVVM_INTERFACES_ITEMCOPYSTRATEGYINTERFACE_H

#include "mvvm/model_export.h"

#include <memory>

namespace mvvm
{
class SessionItem;

//! Interface for deep item copying.

class MVVM_MODEL_EXPORT ItemCopyStrategyInterface
{
public:
  virtual ~ItemCopyStrategyInterface() = default;

  //! Creates item copy by deep copying all children.
  virtual std::unique_ptr<SessionItem> CreateCopy(const SessionItem* item) const = 0;
};

}  // namespace mvvm

#endif  // MVVM_INTERFACES_ITEMCOPYSTRATEGYINTERFACE_H
