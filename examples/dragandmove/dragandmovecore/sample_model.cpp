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

#include "sampleitems.h"

#include <mvvm/utils/numeric_utils.h>
#include <mvvm/widgets/widget_utils.h>

namespace DragAndMove
{

namespace
{
std::string GetRandomName()
{
  static const std::string alphabet = "abcdefgh";
  const size_t len(3);

  std::string result;
  for (size_t i = 0; i < len; ++i)
  {
    size_t random_index =
        static_cast<size_t>(mvvm::utils::RandInt(0, static_cast<int>(alphabet.size() - 1)));
    result.push_back(alphabet[random_index]);
  }

  return result;
}
}  // namespace

SampleModel::SampleModel() : mvvm::ApplicationModel("SampleModel")
{
  RegisterItem<DemoItem>();
  RegisterItem<DemoContainerItem>();
//  PopulateModel();
  SetUndoEnabled(true);
}

void SampleModel::AppendRandomItem(mvvm::SessionItem* container)
{
  //auto item = InsertItem<DemoItem>(container);

  // First fill item, then insert. This is done to provide possibility to undo in one step.
  // Will be replace as soon as undo macros are ready.

  auto item = std::make_unique<DemoItem>();
  item->SetProperty(DemoItem::P_COLOR_PROPERTY, mvvm::utils::RandomNamedColor());
  item->SetProperty(DemoItem::P_STRING_PROPERTY, GetRandomName());
  item->SetProperty(DemoItem::P_INTEGER_PROPERTY, mvvm::utils::RandInt(0, 10));

  InsertItem(std::move(item), container, {"", -1});
}

//! Generates initial model content.

void SampleModel::PopulateModel()
{
  auto container = InsertItem<DemoContainerItem>();
  AppendRandomItem(container);
  AppendRandomItem(container);
  AppendRandomItem(container);

  container = InsertItem<DemoContainerItem>();
  AppendRandomItem(container);
  AppendRandomItem(container);
}

}  // namespace DragAndMove
