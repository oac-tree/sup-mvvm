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

#include "demo_item.h"

#include <mvvm/model/item_utils.h>
#include <mvvm/standarditems/editor_constants.h>
#include <mvvm/widgets/widget_utils.h>

#include <algorithm>
#include <chrono>
#include <random>

namespace
{

std::string GetRandomName()
{
  std::string alphabet{"abcdefghijklmnopqrstuvwxyz"};
  const unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::shuffle(alphabet.begin(), alphabet.end(), std::default_random_engine(seed));
  return alphabet.substr(0, 6);
}

}  // namespace

namespace flatview
{

DemoItem::DemoItem() : mvvm::CompoundItem("Item")
{
  AddProperty("Name", GetRandomName());
  AddProperty("Available", true)
      .SetToolTip("tooltip")
      .SetStrategy(this, &DemoItem::OnSetAvailableFlag);
  AddProperty("Answer", 42);
  AddProperty("Distance", 42.1234).SetDisplayName("Double");
  AddProperty("Options", mvvm::ComboProperty({"option 1", "option 2", "option 3"}));
  AddProperty("Access", mvvm::utils::RandomNamedColor())
      .SetEditorType(mvvm::constants::kColorEditorType);
}

bool DemoItem::OnSetAvailableFlag(SessionItem* property, const mvvm::variant_t& variant,
                                  mvvm::role_t role)
{
  mvvm::utils::BeginMacro(*this, "SetAvailableFlag");

  // change Available flag itself
  const bool result = mvvm::utils::SetData(*property, variant, role);

  // marking property "Answer" disabled if "Available" flag is false
  GetItem("Answer").SetEnabled(Property<bool>("Available"));

  mvvm::utils::EndMacro(*this);

  return result;
}

}  // namespace flatview
