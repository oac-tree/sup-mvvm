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
#include "mvvm/editors/coloreditor.h"
#include "mvvm/editors/combopropertyeditor.h"
#include "mvvm/model/sessionitem.h"

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

editorbuilder_t ColorEditorBuilder()
{
  auto builder = [](const SessionItem*) -> editor_t { return std::make_unique<ColorEditor>(); };
  return builder;
}

}  // namespace mvvm
