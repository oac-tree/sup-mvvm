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
      .SetEditorType(mvvm::constants::kScientificSpinboxEditorType);

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
  // Here we want to compare visual appearance of QSpinBox editor and AllIntSpinBoxEditor.
  // QSpinBox is the default for int32, AllIntSpinBox is the default for the rest.

  // QSpinBox editor for int32
  AddProperty("integer_1", 42).SetDisplayName("int32 QSpinBox");

  // QSpinBox editor for int32 with limits set
  auto& integer_2_prop = AddProperty("integer_2", 42).SetDisplayName("int32 QSpinBox lim");
  mvvm::SetLimited(30, 40, integer_2_prop);

  // AllIntSpinBoxEditor for int32 should be set manually
  AddProperty("integer_3", 42)
      .SetDisplayName("int32 AllIntEditor")
      .SetEditorType(mvvm::constants::kAllIntSpinBoxEditorType);

  // AllIntSpinBoxEditor with limits, for int32 should be set manually
  auto& integer_4_prop = AddProperty("integer_4", 42)
                             .SetDisplayName("int32 AllIntEditor lim")
                             .SetEditorType(mvvm::constants::kAllIntSpinBoxEditorType);
  mvvm::SetLimited(30, 40, integer_4_prop);

  // AllIntSpinBoxEditor for int8
  AddProperty("integer_5", static_cast<mvvm::int8>(0)).SetDisplayName("int8 AllIntEditor");

  // AllIntSpinBoxEditor for int64
  AddProperty("integer_6", static_cast<mvvm::int64>(0)).SetDisplayName("int64 AllIntEditor");
}

// ------------------------------------------------------------------------------------------------
// FloatPropertyItem
// ------------------------------------------------------------------------------------------------

FloatPropertyItem::FloatPropertyItem() : mvvm::CompoundItem("Float")
{
  // Here we want to compare visual appearance of QDoubleSpinBox and FloatSpinBox.
  // FloatSpinBox is the default for float32 and float64, QDoubleSpinBox should be set manually.

  AddProperty("double_1", 42.1)
      .SetEditorType(mvvm::constants::kDoubleEditorType)
      .SetDisplayName("double QDoubleSpinBox");
  AddProperty("double_2", 42.2)
      .SetEditorType(mvvm::constants::kDoubleEditorType)
      .SetDisplayName("double QDoubleSpinBox");
  auto& double_3_prop = AddProperty("double_3", 42.3).SetDisplayName("double lim QDoubleSpinBox");
  mvvm::SetLimited(30.0, 40.0, double_3_prop);

  // FloatSpinBox for float64
  AddProperty("double_4", static_cast<mvvm::float64>(42.1)).SetDisplayName("float64 FloatSpinBox");

  // FloatSpinBox with limits, for float64
  AddProperty("double_5", static_cast<mvvm::float64>(42.2)).SetDisplayName("float64 FloatSpinBox");

  auto& double_6_prop = AddProperty("double_6", static_cast<mvvm::float64>(42.3))
                            .SetDisplayName("float64 lim FloatSpinBox");
  mvvm::SetLimited(30.0, 40.0, double_6_prop);

  // FloatSpinBox for float32
  AddProperty("double_7", static_cast<mvvm::float32>(42.3))
      .SetDisplayName("float32 lim FloatSpinBox");
}

// ------------------------------------------------------------------------------------------------
// CheckboxItem
// ------------------------------------------------------------------------------------------------

CheckboxItem::CheckboxItem()
{
  auto& int_prop = AddProperty("Integer with checkbox", 42);
  int_prop.SetData(Qt::Checked, mvvm::DataRole::kCheckState);

  AddProperty("Bool with checkbox", true);
  AddProperty("Another bool", false);
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

mvvm::ContainerItem* SampleModel::GetContainer(std::uint32_t index) const
{
  auto containers = mvvm::utils::GetTopItems<mvvm::ContainerItem>(*this);
  if (index >= containers.size())
  {
    throw std::out_of_range("Container index is out of range");
  }
  return containers.at(index);
}

}  // namespace celleditors
