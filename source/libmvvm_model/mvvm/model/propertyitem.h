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

#ifndef MVVM_MODEL_PROPERTYITEM_H
#define MVVM_MODEL_PROPERTYITEM_H

#include "mvvm/model/sessionitem.h"

namespace mvvm
{
class RealLimits;

//! Item to carry concrete editable entity (e.g. 'double' value with limits).
//! Intended for use as a child or CompountItem, not expected to have own children.

class MVVM_MODEL_EXPORT PropertyItem : public SessionItem
{
public:
  static inline const std::string Type = "PropertyItem";

  PropertyItem();

  PropertyItem* SetDisplayName(const std::string& name) override;

  //    PropertyItem* setLimits(const RealLimits& value);
};

}  // namespace mvvm

#endif  // MVVM_MODEL_PROPERTYITEM_H
