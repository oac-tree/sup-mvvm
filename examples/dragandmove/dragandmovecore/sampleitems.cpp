// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

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
