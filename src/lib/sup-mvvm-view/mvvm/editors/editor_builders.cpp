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

#include "editor_builders.h"

#include "custom_editor_includes.h"

#include <mvvm/model/item_limits_helper.h>
#include <mvvm/model/session_item.h>
#include <mvvm/providers/viewmodel_utils.h>
#include <mvvm/standarditems/editor_constants.h>
#include <mvvm/utils/limited_float_helper.h>
#include <mvvm/utils/limited_integer_helper.h>

#include <QDoubleSpinBox>
#include <cmath>

namespace
{

//! Returns 1% of the float data of the item as a step, 1.0 for zero or other data. An item without
//! data uses its default value.
double GetFloatStep(const mvvm::SessionItem& item)
{
  const auto data =
      item.HasData(mvvm::DataRole::kData) ? item.Data() : item.Data(mvvm::DataRole::kDefaultValue);
  double value{0.0};
  if (std::holds_alternative<mvvm::float32>(data))
  {
    value = std::get<mvvm::float32>(data);
  }
  else if (std::holds_alternative<mvvm::float64>(data))
  {
    value = std::get<mvvm::float64>(data);
  }
  return (value == 0.0) ? 1.0 : std::abs(value) / 100.0;
}

}  // namespace

namespace mvvm
{

editorbuilder_t BoolEditorBuilder()
{
  auto builder = [](const SessionItem*) -> editor_t { return std::make_unique<BoolEditor>(); };
  return builder;
}
editorbuilder_t ComboPropertyEditorBuilder()
{
  auto builder = [](const SessionItem*) -> editor_t
  { return std::make_unique<ComboPropertyEditor>(); };
  return builder;
}

editorbuilder_t SelectableComboPropertyEditorBuilder()
{
  auto builder = [](const SessionItem*) -> editor_t
  { return std::make_unique<SelectableComboBoxEditor>(); };
  return builder;
}

editorbuilder_t ColorEditorBuilder()
{
  auto builder = [](const SessionItem*) -> editor_t { return std::make_unique<ColorEditor>(); };
  return builder;
}

editorbuilder_t ExternalPropertyEditorBuilder()
{
  auto builder = [](const SessionItem*) -> editor_t
  { return std::make_unique<ExternalPropertyEditor>(); };
  return builder;
}

editorbuilder_t FloatEditorBuilder()
{
  auto builder = [](const SessionItem* item) -> editor_t
  {
    auto editor = std::make_unique<ScientificSpinBox>();
    if (item == nullptr)
    {
      return editor;
    }

    editor->SetPrecision(utils::GetPrecision(*item));
    editor->SetScientific(item->HasFlag(Appearance::kScientific));
    editor->SetSingleStep(GetFloatStep(*item));

    if (item->HasFlag(Appearance::kNullable))
    {
      // the value is set later by the delegate, the type comes from the default value
      editor->SetLimitedFloat(CreateNullableLimitedFloat(
          {}, item->Data(DataRole::kDefaultValue), GetLowerLimit(*item), GetUpperLimit(*item)));
      editor->SetPlaceholderText(utils::GetPlaceholderText(*item));
    }
    else
    {
      editor->SetRange(GetLowerLimit(*item), GetUpperLimit(*item));
    }
    return editor;
  };
  return builder;
}

editorbuilder_t IntegerEditorBuilder()
{
  auto builder = [](const SessionItem* item) -> editor_t
  {
    auto editor = std::make_unique<QSpinBox>();
    if (item)
    {
      auto limits = GetInt32Limits(*item);
      editor->setRange(limits.first, limits.second);
    }
    return editor;
  };
  return builder;
}

editorbuilder_t AllIntSpinBoxEditorBuilder()
{
  auto builder = [](const SessionItem* item) -> editor_t
  {
    auto editor = std::make_unique<AllIntSpinBox>();
    if (item == nullptr)
    {
      return editor;
    }

    if (item->HasFlag(Appearance::kNullable))
    {
      // the value is set later by the delegate, the type comes from the default value
      editor->SetLimitedInteger(CreateNullableLimitedInteger(
          {}, item->Data(DataRole::kDefaultValue), GetLowerLimit(*item), GetUpperLimit(*item)));
      editor->SetPlaceholderText(utils::GetPlaceholderText(*item));
    }
    else
    {
      editor->SetRange(GetLowerLimit(*item), GetUpperLimit(*item));
    }
    return editor;
  };
  return builder;
}

editorbuilder_t DoubleEditorBuilder()
{
  auto builder = [](const SessionItem* item) -> editor_t
  {
    auto editor = std::make_unique<QDoubleSpinBox>();
    editor->setDecimals(constants::kDefaultDoubleDecimals);
    if (item)
    {
      auto limits = GetFloat64Limits(*item);
      editor->setRange(limits.first, limits.second);
    }
    return editor;
  };
  return builder;
}

}  // namespace mvvm
