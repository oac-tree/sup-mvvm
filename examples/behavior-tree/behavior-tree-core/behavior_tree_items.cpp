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

#include "behavior_tree_items.h"

namespace behaviortree
{

SequenceItem::SequenceItem() : NodeItem(mvvm::GetTypeName<SequenceItem>())
{
  SetDisplayName("Sequence");
  SetBaseColor("royalblue");

  RegisterMultipleChildrenTag();
}

WaitItem::WaitItem() : NodeItem(mvvm::GetTypeName<WaitItem>())
{
  SetDisplayName("Wait");
  SetBaseColor("gray");
}

MessageItem::MessageItem() : NodeItem(mvvm::GetTypeName<MessageItem>())
{
  SetDisplayName("Message");
  SetBaseColor("gray");
}

RepeatItem::RepeatItem() : NodeItem(mvvm::GetTypeName<RepeatItem>())
{
  SetDisplayName("Repeat");
  SetBaseColor("lightseagreen");

  RegisterSingleChildTag();
}

}  // namespace behaviortree
