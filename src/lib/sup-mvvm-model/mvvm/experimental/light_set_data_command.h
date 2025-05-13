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

#ifndef MVVM_EXPERIMENTAL_LIGHT_SET_DATA_COMMAND_H_
#define MVVM_EXPERIMENTAL_LIGHT_SET_DATA_COMMAND_H_

#include <mvvm/core/variant.h>
#include <mvvm/experimental/light_command.h>

namespace mvvm::experimental
{

class LightItem;

class LightSetDataCommand : public LightCommand
{
public:
  LightSetDataCommand(LightItem* item, const variant_t& value, int32_t role);

  std::optional<event_variant_t> GetNextEvent() const override;

private:
  void ExecuteImpl() override;
  void UndoImpl() override;

};

}  // namespace mvvm::experimental

#endif  // MVVM_EXPERIMENTAL_LIGHT_ITEM_H_
