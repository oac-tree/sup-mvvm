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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "custom_model.h"

#include <mvvm/model/compound_item.h>
#include <mvvm/model/property_item.h>
#include <mvvm/standarditems/container_item.h>
#include <mvvm/standarditems/editor_constants.h>

namespace customeditors
{

CustomModel::CustomModel()
{
  auto container = InsertItem<mvvm::ContainerItem>();

  // simple string
  auto property0 = InsertItem<mvvm::PropertyItem>(container);
  property0->SetDisplayName("string");
  property0->SetData("hello");

  auto property1 = InsertItem<mvvm::PropertyItem>(container);
  property1->SetDisplayName("string completer");
  property1->SetData(std::string());
  property1->SetEditorType(mvvm::constants::kStringCompleterEditorType);

  auto property2 = InsertItem<mvvm::PropertyItem>(container);
  property2->SetDisplayName("combo completer");
  property2->SetData(std::string());
  property2->SetEditorType(mvvm::constants::kComboCompleterEditorType);
}

}  // namespace customeditors
