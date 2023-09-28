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

#include "sample_model.h"

#include <mvvm/model/item_limits_helper.h>
#include <mvvm/standarditems/editor_constants.h>

#include <QtCore>

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

BasicPropertyItem::BasicPropertyItem() : mvvm::CompoundItem("BasicProperty")
{
  AddProperty(kBool, true)->SetDisplayName("Bool")->SetToolTip("tooltip");
  AddProperty(kInteger, 42)->SetDisplayName("Integer");
  AddProperty(kString, "abc")->SetDisplayName("String");
  AddProperty(kDouble, 42.1234)->SetDisplayName("Double");
  AddProperty(kScientificDouble, 42.12e-09)
      ->SetDisplayName("Scientific")
      ->SetEditorType(mvvm::constants::kScientificSpinboxEditorType);

  AddProperty(kLongInteger, 4294967296L) /* UINT32_MAX + 1 */
      ->SetDisplayName("Long Integer");
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

AllIntPropertyItem::AllIntPropertyItem() : mvvm::CompoundItem("AllInt")
{
  // original QSpinBox editor
  AddProperty("integer_1", 42)->SetDisplayName("int32 QSpinBox");

  // original QSpinBox editor with limits set
  auto prop = AddProperty("integer_2", 42)->SetDisplayName("int32 QSpinBox lim");
  mvvm::SetLimited(30, 40, *prop);

  // new AllIntSpinBoxEditor
  AddProperty("integer_3", 42)
      ->SetDisplayName("int32 AllIntEditor")
      ->SetEditorType(mvvm::constants::kAllIntSpinBoxEditorType);

  // new AllIntSpinBoxEditor with limits set
  prop = AddProperty("integer_4", 42)->SetDisplayName("int32 AllIntEditor lim");
  prop->SetEditorType(mvvm::constants::kAllIntSpinBoxEditorType);
  mvvm::SetLimited(30, 40, *prop);

  // new AllIntSpinBoxEditor with small numbers (int8)
  AddProperty("integer_5", static_cast<mvvm::int8>(0))
      ->SetEditorType(mvvm::constants::kAllIntSpinBoxEditorType)
      ->SetDisplayName("int8 AllIntEditor");

  // LongIntSpinBoxEditor with int64
  AddProperty("integer_6", static_cast<mvvm::int64>(0))
      ->SetEditorType(mvvm::constants::kLongIntSpinBoxEditorType)
      ->SetDisplayName("int64 LongIntEditor");
}

FloatPropertyItem::FloatPropertyItem() : mvvm::CompoundItem("Float")
{
  // original QSpinBox editor
  AddProperty("double_1", 42.1)->SetDisplayName("double QDoubleSpinBox");
  AddProperty("double_2", 42.2)->SetDisplayName("double QDoubleSpinBox");
  auto prop = AddProperty("double_3", 42.3)->SetDisplayName("double lim QDoubleSpinBox");
  mvvm::SetLimited(30.0, 40.0, *prop);

  AddProperty("double_4", static_cast<mvvm::float64>(42.1))
      ->SetEditorType(mvvm::constants::kFloatSpinBoxEditorType)
      ->SetDisplayName("float64 FloatSpinBox");
  AddProperty("double_5", static_cast<mvvm::float64>(42.2))
      ->SetEditorType(mvvm::constants::kFloatSpinBoxEditorType)
      ->SetDisplayName("float64 FloatSpinBox");
  prop = AddProperty("double_6", static_cast<mvvm::float64>(42.3))
             ->SetDisplayName("float64 lim FloatSpinBox");
  prop->SetEditorType(mvvm::constants::kFloatSpinBoxEditorType);
  mvvm::SetLimited(30.0, 40.0, *prop);
}

SampleModel::SampleModel() : mvvm::ApplicationModel("SampleModel")
{
  RegisterItem<BasicPropertyItem>();
  RegisterItem<AllIntPropertyItem>();
  RegisterItem<FloatPropertyItem>();
}

}  // namespace celleditors
