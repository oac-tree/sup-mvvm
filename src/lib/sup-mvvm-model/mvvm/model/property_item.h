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

#ifndef MVVM_MODEL_PROPERTY_ITEM_H_
#define MVVM_MODEL_PROPERTY_ITEM_H_

#include <mvvm/model/sessionitem.h>

namespace mvvm
{

/**
 * @brief The PropertyItem class is an item to carry concrete editable entity (e.g. float64
 * value with limits).
 *
 * Intended for use as a child or CompoundItem, not expected to have own children. For the moment,
 * it has no extra behavior and it is here for historical reasons.
 */
class MVVM_MODEL_EXPORT PropertyItem : public SessionItem
{
public:
  static inline const std::string Type = "PropertyItem";
  using SessionItem::SessionItem;

  PropertyItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id = true) const override;
};

}  // namespace mvvm

#endif  // MVVM_MODEL_PROPERTY_ITEM_H_
