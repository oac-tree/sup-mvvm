/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
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

#ifndef MVVM_SERIALIZATION_SESSION_ITEM_COMPOSER_H_
#define MVVM_SERIALIZATION_SESSION_ITEM_COMPOSER_H_

#include <mvvm/serialization/converter_types.h>
#include <mvvm/serialization/tree_data_fwd.h>

#include <memory>

namespace mvvm
{

class SessionItem;

/**
 * @brief The SessionItemComposer class constructs SessionItem from its TreeData representation.
 */
class SessionItemComposer
{
public:
  explicit SessionItemComposer(const ItemComposerContext& context);

  std::unique_ptr<SessionItem> CreateSessionItem(const tree_data_t& tree_data) const;

private:
  ItemComposerContext m_context;
};

}  // namespace mvvm

#endif  // MVVM_SERIALIZATION_SESSION_ITEM_COMPOSER_H_
