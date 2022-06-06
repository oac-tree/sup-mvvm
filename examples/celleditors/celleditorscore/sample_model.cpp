// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#include "sample_model.h"

#include "mvvm/standarditems/editor_constants.h"

namespace
{
const std::string kBool = "kBool";
const std::string kInteger = "kInteger";
const std::string kString = "kString";
const std::string kDouble = "kDouble";
const std::string kColor = "kColor";
const std::string kCombo = "kCombo";
const std::string kSelectableCombo = "kSelectableCombo";
const std::string kExternal = "kExternal";
const std::string kScientificDouble = "kScientificDouble";
}  // namespace

namespace celleditors
{

DemoItem::DemoItem() : mvvm::CompoundItem("DemoItem")
{
  AddProperty(kBool, true)->SetDisplayName("Bool")->SetToolTip("tooltip");
  AddProperty(kInteger, 42)->SetDisplayName("Integer");
  AddProperty(kString, "abc")->SetDisplayName("String");
  AddProperty(kDouble, 42.1234)->SetDisplayName("Double");
  AddProperty(kScientificDouble, 42.12e-09)
      ->SetDisplayName("Scientific")
      ->SetEditorType(mvvm::constants::kScientificSpinboxEditorType);
  AddProperty(kColor, "green")
      ->SetDisplayName("Color")
      ->SetEditorType(mvvm::constants::kColorEditorType);
  AddProperty(kCombo, mvvm::ComboProperty({"option 1", "option 2", "option 3"}))
      ->SetDisplayName("Combo");
  AddProperty(kSelectableCombo, mvvm::ComboProperty({"option 1", "option 2", "option 3"}))
      ->SetDisplayName("Selectable")
      ->SetEditorType(mvvm::constants::kSelectableComboPropertyEditorType);
  AddProperty(kExternal, mvvm::ExternalProperty({"text", "gold"}))->SetDisplayName("External");
}

SampleModel::SampleModel() : mvvm::ApplicationModel("SampleModel")
{
  RegisterItem<DemoItem>();
}

}  // namespace celleditors
