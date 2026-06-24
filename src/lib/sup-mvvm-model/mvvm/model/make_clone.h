/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_MODEL_MAKE_CLONE_H_
#define MVVM_MODEL_MAKE_CLONE_H_

#include <mvvm/model/session_item.h>
#include <mvvm/model/session_item_data.h>
#include <mvvm/model/tagged_items.h>

#include <memory>

namespace mvvm
{

/**
 * @brief Creates a clone of the given item, preserving its type and all data.
 *
 * The clone is built from a default-constructed object of the same type, whose content is then
 * replaced by deep copies of the source's data and tags. This avoids relying on a copy constructor,
 * which the SessionItem hierarchy intentionally deletes. Item identifiers are preserved (use
 * utils::CopyItem to obtain a deep copy with regenerated identifiers).
 *
 * @tparam T The concrete item type to clone.
 * @param source The item to clone.
 * @return The cloned item.
 */
template <typename T>
std::unique_ptr<T> MakeClone(const T& source)
{
  auto result = std::make_unique<T>();

  // Replace freshly constructed content with deep copies of the source's content. The source type
  // is passed explicitly to preserve type strings, including the case of a subclass cloned through
  // a base class Clone() override.
  result->SetDataAndTags(source.GetType(),
                         std::make_unique<SessionItemData>(*source.GetItemData()),
                         source.GetTaggedItems()->Clone());

  // TaggedItems::Clone() does not repoint children to the new owner.
  for (auto* child : result->GetAllItems())
  {
    child->SetParent(result.get());
  }

  return result;
}

}  // namespace mvvm

#endif  // MVVM_MODEL_MAKE_CLONE_H_
