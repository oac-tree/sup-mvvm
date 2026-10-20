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

#include "sample_model.h"

#include <mvvm/model/item_limits_helper.h>
#include <mvvm/model/model_utils.h>
#include <mvvm/model/property_item.h>
#include <mvvm/standarditems/container_item.h>
#include <mvvm/standarditems/editor_constants.h>

#include <Qt>

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
const std::string kLongInteger = "kLongInteger";
}  // namespace

namespace celleditors
{

// ------------------------------------------------------------------------------------------------
// BasicPropertyItem
// ------------------------------------------------------------------------------------------------

BasicPropertyItem::BasicPropertyItem() : mvvm::CompoundItem("BasicProperty")
{
  AddProperty(kBool, true)
      .SetDisplayName("Bool")
      .SetToolTip("tooltip")
      .SetFlag(mvvm::Appearance::kEditableDisplayName, true);
  AddProperty(kInteger, 42).SetDisplayName("Integer");
  AddProperty(kString, "abc").SetDisplayName("String");
  AddProperty(kDouble, 42.1234).SetDisplayName("Double");
  AddProperty(kScientificDouble, 42.12e-09)
      .SetDisplayName("Scientific")
      .SetFlag(mvvm::Appearance::kScientific, true);

  AddProperty(kColor, "green")
      .SetDisplayName("Color")
      .SetEditorType(mvvm::constants::kColorEditorType);

  mvvm::ComboProperty combo({"option 1", "option 2", "option 3"});
  combo.SetToolTips({"tooltip1", "tooltip2", "tooltip3"});
  AddProperty(kCombo, combo).SetDisplayName("Combo");

  AddProperty(kSelectableCombo, combo)
      .SetDisplayName("Selectable")
      .SetEditorType(mvvm::constants::kSelectableComboPropertyEditorType);
  AddProperty(kExternal, mvvm::ExternalProperty({"text", "gold"})).SetDisplayName("External");
}

// ------------------------------------------------------------------------------------------------
// AllIntPropertyItem
// ------------------------------------------------------------------------------------------------

AllIntPropertyItem::AllIntPropertyItem() : mvvm::CompoundItem("AllInt")
{
  // Here we want to compare visual appearance of QSpinBox editor and AllIntSpinBox.
  // QSpinBox is the default for int32, AllIntSpinBox is the default for the rest.

  // QSpinBox editor for int32
  AddProperty("integer_1", 42).SetDisplayName("int32 QSpinBox");

  // QSpinBox editor for int32 with limits set
  auto& integer_2_prop = AddProperty("integer_2", 42).SetDisplayName("int32 QSpinBox lim");
  mvvm::SetLimited(30, 40, integer_2_prop);

  // AllIntSpinBox for int32 should be set manually
  AddProperty("integer_3", 42)
      .SetDisplayName("int32 AllIntEditor")
      .SetEditorType(mvvm::constants::kAllIntSpinBoxEditorType);

  // AllIntSpinBox with limits, for int32 should be set manually
  auto& integer_4_prop = AddProperty("integer_4", 42)
                             .SetDisplayName("int32 AllIntEditor lim")
                             .SetEditorType(mvvm::constants::kAllIntSpinBoxEditorType);
  mvvm::SetLimited(30, 40, integer_4_prop);

  // AllIntSpinBox for int8
  AddProperty("integer_5", static_cast<mvvm::int8>(0)).SetDisplayName("int8 AllIntEditor");

  // AllIntSpinBox for int64
  AddProperty("integer_6", static_cast<mvvm::int64>(0)).SetDisplayName("int64 AllIntEditor");
}

// ------------------------------------------------------------------------------------------------
// OptionalIntPropertyItem
// ------------------------------------------------------------------------------------------------

OptionalIntPropertyItem::OptionalIntPropertyItem() : mvvm::CompoundItem("OptionalInt")
{
  // int8 without value
  auto& int8_prop = AddProperty<mvvm::PropertyItem>("optional_1");
  int8_prop.SetDisplayName("int8 no value").SetFlag(mvvm::Appearance::kNullable, true);
  int8_prop.SetData(mvvm::int8{0}, mvvm::DataRole::kDefaultValue);

  // int32 with value, AllIntSpinBox instead of QSpinBox
  auto& int32_prop = AddProperty("optional_2", 42);
  int32_prop.SetDisplayName("int32 value").SetFlag(mvvm::Appearance::kNullable, true);
  int32_prop.SetData(0, mvvm::DataRole::kDefaultValue);

  // uint32 without value, with limits and own placeholder text
  auto& uint32_prop = AddProperty<mvvm::PropertyItem>("optional_3");
  uint32_prop.SetDisplayName("uint32 no value lim").SetFlag(mvvm::Appearance::kNullable, true);
  uint32_prop.SetData(mvvm::uint32{15}, mvvm::DataRole::kDefaultValue);
  uint32_prop.SetData(std::string("auto"), mvvm::DataRole::kPlaceholder);
  mvvm::SetLimited(mvvm::uint32{10}, mvvm::uint32{20}, uint32_prop);

  // int64 without value
  auto& int64_prop = AddProperty<mvvm::PropertyItem>("optional_4");
  int64_prop.SetDisplayName("int64 no value").SetFlag(mvvm::Appearance::kNullable, true);
  int64_prop.SetData(mvvm::int64{0}, mvvm::DataRole::kDefaultValue);
}

// ------------------------------------------------------------------------------------------------
// FloatPropertyItem
// ------------------------------------------------------------------------------------------------

FloatPropertyItem::FloatPropertyItem() : mvvm::CompoundItem("Float")
{
  // Here we want to compare visual appearance of QDoubleSpinBox and ScientificSpinBox.
  // ScientificSpinBox is the default for float32 and float64, QDoubleSpinBox should be set
  // manually.

  AddProperty("double_1", 42.1)
      .SetEditorType(mvvm::constants::kDoubleEditorType)
      .SetDisplayName("double QDoubleSpinBox");
  AddProperty("double_2", 42.2)
      .SetEditorType(mvvm::constants::kDoubleEditorType)
      .SetDisplayName("double QDoubleSpinBox");
  auto& double_3_prop = AddProperty("double_3", 42.3)
                            .SetEditorType(mvvm::constants::kDoubleEditorType)
                            .SetDisplayName("double lim QDoubleSpinBox");
  mvvm::SetLimited(30.0, 40.0, double_3_prop);

  // ScientificSpinBox for float64
  AddProperty("double_4", static_cast<mvvm::float64>(42.1)).SetDisplayName("float64");

  // ScientificSpinBox in exponential notation, for float64
  AddProperty("double_5", static_cast<mvvm::float64>(42.2))
      .SetDisplayName("float64 scientific")
      .SetFlag(mvvm::Appearance::kScientific, true);

  // ScientificSpinBox with limits, for float64
  auto& double_6_prop =
      AddProperty("double_6", static_cast<mvvm::float64>(42.3)).SetDisplayName("float64 lim");
  mvvm::SetLimited(30.0, 40.0, double_6_prop);

  // ScientificSpinBox for float32
  AddProperty("double_7", static_cast<mvvm::float32>(42.3)).SetDisplayName("float32");
}

// ------------------------------------------------------------------------------------------------
// CheckboxItem
// ------------------------------------------------------------------------------------------------

CheckboxItem::CheckboxItem()
{
  // to see how integer behaves when it is equipped with aditional checkbox
  auto& int_prop = AddProperty("Integer with checkbox", 42);
  int_prop.SetData(Qt::Checked, mvvm::DataRole::kCheckState);

  // property which is bool by itself, so it should have checkbox by default
  AddProperty("Bool with checkbox", true);
}

// ------------------------------------------------------------------------------------------------
// SampleModel
// ------------------------------------------------------------------------------------------------

SampleModel::SampleModel() : mvvm::ApplicationModel("SampleModel") {}

void SampleModel::PopulateModel()
{
  auto container0 = InsertItem<mvvm::ContainerItem>();
  container0->SetDisplayName("Basic properties");
  InsertItem<BasicPropertyItem>(container0);
  InsertItem<BasicPropertyItem>(container0);
  InsertItem<BasicPropertyItem>(container0);

  auto container1 = InsertItem<mvvm::ContainerItem>();
  container1->SetDisplayName("All integers");
  InsertItem<AllIntPropertyItem>(container1);
  InsertItem<AllIntPropertyItem>(container1);
  InsertItem<AllIntPropertyItem>(container1);

  auto container_optional = InsertItem<mvvm::ContainerItem>();
  container_optional->SetDisplayName("Optional integers");
  InsertItem<OptionalIntPropertyItem>(container_optional);
  InsertItem<OptionalIntPropertyItem>(container_optional);
  InsertItem<OptionalIntPropertyItem>(container_optional);

  auto container2 = InsertItem<mvvm::ContainerItem>();
  container2->SetDisplayName("Floats");
  InsertItem<FloatPropertyItem>(container2);
  InsertItem<FloatPropertyItem>(container2);
  InsertItem<FloatPropertyItem>(container2);

  auto container3 = InsertItem<mvvm::ContainerItem>();
  container3->SetDisplayName("Checkboxes");
  InsertItem<CheckboxItem>(container3);
  InsertItem<CheckboxItem>(container3);
  InsertItem<CheckboxItem>(container3);
}

std::uint32_t SampleModel::GetContainerCount() const
{
  return GetRootItem()->GetTotalItemCount();
}

mvvm::ContainerItem* SampleModel::GetContainer(std::uint32_t index)
{
  auto containers = mvvm::utils::GetTopItems<mvvm::ContainerItem>(*this);
  if (index >= containers.size())
  {
    throw std::out_of_range("Container index is out of range");
  }
  return containers.at(index);
}

}  // namespace celleditors
