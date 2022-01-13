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

#ifndef MVVM_STANDARDITEMS_CONTAINERITEM_H
#define MVVM_STANDARDITEMS_CONTAINERITEM_H

#include "mvvm/model/compounditem.h"

namespace mvvm
{
//! Simple container to store any type of children.
//! Used as convenience item to create branch with uniform children beneath.

class MVVM_MODEL_EXPORT ContainerItem : public CompoundItem
{
public:
  static inline const std::string Type = "ContainerItem";

  ContainerItem(const std::string& model_type = Type);

  bool IsEmpty() const;
};

}  // namespace mvvm

#endif  // MVVM_STANDARDITEMS_CONTAINERITEM_H
