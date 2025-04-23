/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
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

#include "mousemodel.h"

#include <mvvm/commands/i_command_stack.h>
#include <mvvm/factories/model_document_factory.h>
#include <mvvm/model/item_factory.h>
#include <mvvm/standarditems/editor_constants.h>
#include <mvvm/widgets/widget_utils.h>

#include <QColor>
#include <algorithm>
#include <cmath>

namespace
{
const int MouseCount = 7;

bool RegisterItems()
{
  mvvm::RegisterGlobalItem<collidingmice::MouseItem>();
  return true;
}

static bool items_registered_flag = RegisterItems();

}  // namespace

namespace collidingmice
{

MouseItem::MouseItem() : mvvm::CompoundItem("MouseItem")
{
  AddProperty(P_COLOR, "red")
      .SetDisplayName("Color")
      .SetEditorType(mvvm::constants::kColorEditorType);
  AddProperty(P_XPOS, 0.0).SetDisplayName("X");
  AddProperty(P_YPOS, 0.0).SetDisplayName("Y");
  AddProperty(P_ANGLE, 0.0).SetDisplayName("Angle of yaw");
  AddProperty(P_SPEED, 0.0).SetDisplayName("Speed");
}

// ----------------------------------------------------------------------------

MouseModel::MouseModel() : mvvm::ApplicationModel("MouseModel")
{
  PopulateModel();

  const int max_commands_to_keep = 5000;
  SetUndoEnabled(true, max_commands_to_keep);
}

void MouseModel::LoadFromFile(const QString& name)
{
  auto document = mvvm::CreateXmlDocument({this}, "MouseApp");
  document->Load(name.toStdString());
}

void MouseModel::SaveToFile(const QString& name)
{
  auto document = mvvm::CreateXmlDocument({this}, "MouseApp");
  document->Save(name.toStdString());
}

void MouseModel::SetUndoPosition(int value)
{
  auto commands = GetCommandStack();
  int desired_command_id = commands->GetCommandCount() * std::clamp(value, 0, 100) / 100;

  if (commands->GetIndex() < desired_command_id)
  {
    while (commands->GetIndex() != desired_command_id)
    {
      commands->Redo();  // going forward
    }
  }
  else
  {
    while (commands->GetIndex() != desired_command_id)
    {
      commands->Undo();  // going back in time
    }
  }
}

void MouseModel::PopulateModel()
{
  for (int i = 0; i < MouseCount; ++i)
  {
    auto item = InsertItem<MouseItem>();
    item->SetProperty(MouseItem::P_XPOS, std::sin((i * 6.28) / MouseCount) * 200);
    item->SetProperty(MouseItem::P_YPOS, std::cos((i * 6.28) / MouseCount) * 200);
    item->SetProperty(MouseItem::P_COLOR, mvvm::utils::RandomNamedColor());
  }
}

}  // namespace collidingmice
