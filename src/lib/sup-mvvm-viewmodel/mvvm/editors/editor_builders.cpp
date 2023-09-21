/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "editor_builders.h"

#include <mvvm/editors/custom_editor_includes.h>
#include <mvvm/model/item_limits_helper.h>
#include <mvvm/model/sessionitem.h>
#include <mvvm/standarditems/editor_constants.h>

#include <QDoubleSpinBox>
#include <QSpinBox>
#include <cmath>

namespace
{
double singleStep(int decimals)
{
  // For item with decimals=3 (i.e. 0.001) single step will be 0.1
  return 1. / std::pow(10., decimals - 1);
}

double getStep(double val)
{
  return val == 0.0 ? 1.0 : val / 100.;
}

namespace
{
const int kMaxDefaultEditableIntegerValue = 65536;
const int kMinDefaultEditableIntegerValue = -kMaxDefaultEditableIntegerValue;
}  // namespace

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

editorbuilder_t ScientificSpinBoxEditorBuilder()
{
  auto builder = [](const SessionItem* item) -> editor_t
  {
    auto editor = std::make_unique<ScientificSpinBoxEditor>();
    if (item)
    {
      if (item->HasData(DataRole::kLimits))
      {
        auto limits = item->Data<RealLimits>(DataRole::kLimits);
        editor->SetRange(limits.GetLowerLimit(), limits.GetUpperLimit());
      }
      editor->SetSingleStep(getStep(item->Data<double>()));
    }
    editor->SetDecimals(constants::kDefaultDoubleDecimals);
    return editor;
  };
  return builder;
}

editorbuilder_t LongIntSpinBoxEditorBuilder()
{
  auto builder = [](const SessionItem* item) -> editor_t
  {
    auto editor = std::make_unique<LongIntSpinBoxEditor>();
    if (item)
    {
      if (item->HasData(DataRole::kLimits))
      {
        auto limits = item->Data<Limits<int64>>(DataRole::kLimits);
        editor->SetRange(limits.GetLowerLimit(), limits.GetUpperLimit());
      }
      else
      {
        editor->SetRange(-65536, 65536);
      }
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

editorbuilder_t DoubleEditorBuilder()
{
  auto builder = [](const SessionItem* item) -> editor_t
  {
    auto editor = std::make_unique<QDoubleSpinBox>();
    editor->setDecimals(constants::kDefaultDoubleDecimals);
    if (item)
    {
      if (item->HasData(DataRole::kLimits))
      {
        auto limits = item->Data<RealLimits>(DataRole::kLimits);
        editor->setRange(limits.GetLowerLimit(), limits.GetUpperLimit());
      }
      else
      {
        editor->setRange(kMinDefaultEditableIntegerValue, kMaxDefaultEditableIntegerValue);
      }
    }
    return editor;
  };
  return builder;
}

}  // namespace mvvm
