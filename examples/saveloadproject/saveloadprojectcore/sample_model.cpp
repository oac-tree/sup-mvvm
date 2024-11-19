/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
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

#include <mvvm/model/item_factory.h>
#include <mvvm/model/model_utils.h>
#include <mvvm/standarditems/editor_constants.h>
#include <mvvm/widgets/widget_utils.h>

#include <chrono>
#include <random>

namespace saveloadproject
{

namespace
{

const std::string kItems = "kItems";

std::string GetRandomName()
{
  std::string alphabet{"abcdefghijklmnopqrstuvwxyz"};
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::shuffle(alphabet.begin(), alphabet.end(), std::default_random_engine(seed));
  return alphabet.substr(0, 6);
}

bool RegisterDemoItems()
{
  mvvm::RegisterGlobalItem<DemoItem>();
  mvvm::RegisterGlobalItem<DemoContainerItem>();
  return true;
}

static bool registered_flag = RegisterDemoItems();

}  // namespace

DemoItem::DemoItem() : CompoundItem(Type)
{
  AddProperty("Color", "green").SetEditorType(mvvm::constants::kColorEditorType);
  AddProperty("Bool", true);
  AddProperty("Integer", 42);
  AddProperty("String", "abc");
  AddProperty("Double", 42.1);
}

DemoContainerItem::DemoContainerItem() : CompoundItem(Type)
{
  RegisterTag(mvvm::TagInfo::CreateUniversalTag(kItems, {DemoItem::Type}), /*set_default*/ true);
}

SampleModel::SampleModel() : mvvm::ApplicationModel("SampleModel")
{
  PopulateModel();
  SetUndoEnabled(true);
}

void SampleModel::AppendRandomItem(mvvm::SessionItem* container)
{
  mvvm::utils::BeginMacro(*this, "ItemCreation");
  auto item = InsertItem<DemoItem>(container);
  item->SetProperty("Color", mvvm::utils::RandomNamedColor());
  item->SetProperty("String", GetRandomName());
  item->SetProperty("Integer", mvvm::utils::RandInt(0, 10));
  mvvm::utils::EndMacro(*this);
}

void SampleModel::PopulateModel()
{
  InsertItem<DemoContainerItem>();
}

}  // namespace saveloadproject
