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

#ifndef MVVM_EVENT_TEST_HELPER_H_
#define MVVM_EVENT_TEST_HELPER_H_

#include <mvvm/signals/event_types.h>

namespace mvvm
{

struct EventValueVisitor
{
  std::string operator()(const DataChangedEvent& value);

  std::string operator()(const PropertyChangedEvent& value);

  std::string operator()(const AboutToInsertItemEvent& value);

  std::string operator()(const ItemInsertedEvent& value);

  std::string operator()(const AboutToRemoveItemEvent& value);

  std::string operator()(const ItemRemovedEvent& value);

  std::string operator()(const ModelAboutToBeResetEvent& value);

  std::string operator()(const ModelResetEvent& value);

  std::string operator()(const ModelAboutToBeDestroyedEvent& value);
};

std::string EventToString(const event_variant_t& variant);

}  // namespace mvvm

#endif  // MVVM_EVENT_TEST_HELPER_H_
