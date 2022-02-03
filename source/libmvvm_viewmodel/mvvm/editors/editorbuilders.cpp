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

#include "mvvm/editors/booleditor.h"
#include "mvvm/editors/combopropertyeditor.h"
#include "mvvm/editors/editor_constants.h"
#include "mvvm/model/sessionitem.h"

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

}  // namespace mvvm
