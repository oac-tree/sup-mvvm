/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/editors/editorbuilders.h"

#include "mvvm/editors/customeditorincludes.h"
#include "mvvm/model/sessionitem.h"
#include "mvvm/standarditems/editor_constants.h"

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
      // FIXME restore after RealLimits
      //          if (item->hasData(ItemDataRole::LIMITS)) {
      //              auto limits = item->data<RealLimits>(ItemDataRole::LIMITS);
      //              editor->setRange(limits.lowerLimit(), limits.upperLimit());
      //          }
      editor->SetSingleStep(getStep(item->Data<double>()));
    }
    editor->SetDecimals(constants::default_double_decimals);
    return editor;
  };
  return builder;
}

}  // namespace mvvm
