/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sampleitems.h"

#include <mvvm/model/combo_property.h>
#include <mvvm/model/external_property.h>
#include <mvvm/standarditems/editor_constants.h>

namespace
{

const std::string DemoItemType = "DemoItem";
const std::string DemoContainerItemType = "DemoItemContainer";

}  // namespace

using namespace mvvm;

namespace DragAndMove
{

DemoItem::DemoItem() : CompoundItem(DemoItemType)
{
  AddProperty(P_COLOR_PROPERTY, "green")
      ->SetDisplayName("Color")
      ->SetEditorType(mvvm::constants::kColorEditorType);
  AddProperty(P_BOOL_PROPERTY, true)->SetDisplayName("Bool");
  AddProperty(P_INTEGER_PROPERTY, 42)->SetDisplayName("Integer");
  AddProperty(P_STRING_PROPERTY, "abc")->SetDisplayName("String");
  AddProperty(P_DOUBLE_PROPERTY, 42.1)->SetDisplayName("Double");
}

DemoContainerItem::DemoContainerItem() : CompoundItem(DemoContainerItemType)
{
  RegisterTag(TagInfo::CreateUniversalTag(T_ITEMS, {DemoItemType}), /*set_default*/ true);
}

}  // namespace DragAndMove
