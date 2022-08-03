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

#include <mvvm/model/combo_property.h>
#include <mvvm/standarditems/editor_constants.h>
#include <mvvm/utils/numeric_utils.h>
#include <mvvm/widgets/widget_utils.h>

namespace
{

const std::string kDemoItemType = "DemoItem";
const std::string kDemoContainerItemType = "DemoItemContainer";
const std::string kItems = "kItems";

std::string GetRandomName()
{
  static const std::string alphabet = "abcdefgh";
  const size_t len(3);

  std::string result;
  for (size_t i = 0; i < len; ++i)
  {
    auto random_index =
        static_cast<size_t>(mvvm::utils::RandInt(0, static_cast<int>(alphabet.size() - 1)));
    result.push_back(alphabet[random_index]);
  }

  return result;
}
}  // namespace

namespace dragandmove
{

DemoItem::DemoItem() : CompoundItem(kDemoItemType)
{
  AddProperty("Color", "green")->SetEditorType(mvvm::constants::kColorEditorType);
  AddProperty("Bool", true);
  AddProperty("Integer", 42);
  AddProperty("String", "abc");
  AddProperty("Double", 42.1);
}

DemoContainerItem::DemoContainerItem() : CompoundItem(kDemoContainerItemType)
{
  RegisterTag(mvvm::TagInfo::CreateUniversalTag(kItems, {kDemoItemType}), /*set_default*/ true);
}

SampleModel::SampleModel() : mvvm::ApplicationModel("SampleModel")
{
  RegisterItem<DemoItem>();
  RegisterItem<DemoContainerItem>();
  PopulateModel();
  SetUndoEnabled(true);
}

void SampleModel::AppendRandomItem(mvvm::SessionItem* container)
{
  // auto item = InsertItem<DemoItem>(container);

  // First fill item, then insert. This is done to provide possibility to undo in one step.
  // Will be replace as soon as undo macros are ready.

  auto item = std::make_unique<DemoItem>();
  item->SetProperty("Color", mvvm::utils::RandomNamedColor());
  item->SetProperty("String", GetRandomName());
  item->SetProperty("Integer", mvvm::utils::RandInt(0, 10));

  InsertItem(std::move(item), container, {"", -1});
}

//! Generates initial model content.

void SampleModel::PopulateModel()
{
  auto container = InsertItem<DemoContainerItem>();
  container = InsertItem<DemoContainerItem>();
}

}  // namespace dragandmove
