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

#include <mvvm/standarditems/editor_constants.h>

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
