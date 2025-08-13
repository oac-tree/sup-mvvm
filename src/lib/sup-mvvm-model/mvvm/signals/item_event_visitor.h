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

#ifndef MVVM_SIGNALS_ITEM_EVENT_VISITOR_H_
#define MVVM_SIGNALS_ITEM_EVENT_VISITOR_H_

#include <mvvm/signals/event_types.h>

namespace mvvm
{

/**
 * @brief The ItemEventVisitor is a helper class to visit event_variant_t and report the source of
 * concrete event.
 */
struct ItemEventVisitor
{
  SessionItem* m_source{nullptr};

  void operator()(const DataChangedEvent& event);

  void operator()(const PropertyChangedEvent& event);

  void operator()(const AboutToInsertItemEvent& event);

  void operator()(const ItemInsertedEvent& event);

  void operator()(const AboutToRemoveItemEvent& event);

  void operator()(const ItemRemovedEvent& event);

  void operator()(const ModelAboutToBeResetEvent& event);

  void operator()(const ModelResetEvent& event);

  void operator()(const ModelAboutToBeDestroyedEvent& event);
};

}  // namespace mvvm

#endif  // MVVM_SIGNALS_ITEM_EVENT_VISITOR_H_
