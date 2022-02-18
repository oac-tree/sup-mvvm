// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#include "samplemodel.h"

#include "mvvm/standarditems/editor_constants.h"

namespace celleditors
{

DemoItem::DemoItem() : mvvm::CompoundItem("DemoItem")
{
  AddProperty(P_BOOL_PROPERTY, true)->SetDisplayName("Bool")->SetToolTip("tooltip");
  AddProperty(P_INTEGER_PROPERTY, 42)->SetDisplayName("Integer");
  AddProperty(P_STRING_PROPERTY, "abc")->SetDisplayName("String");
  AddProperty(P_DOUBLE_PROPERTY, 42.1234)->SetDisplayName("Double");
  AddProperty(P_COLOR_PROPERTY, "green")
      ->SetDisplayName("Color")
      ->SetEditorType(mvvm::constants::kColorEditorType);
}

SampleModel::SampleModel() : mvvm::ApplicationModel("SampleModel")
{
  RegisterItem<DemoItem>();
}

}  // namespace celleditors
