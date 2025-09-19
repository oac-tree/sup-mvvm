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

#include "node_item.h"

#include <mvvm/model/item_constants.h>

namespace mvvm
{

NodeItem::NodeItem(const std::string& model_type)
    : CompoundItem(model_type.empty() ? GetTypeName<NodeItem>() : model_type)
{
  (void)AddProperty(kXpos, 0.0).SetDisplayName("X");
  (void)AddProperty(kYpos, 0.0).SetDisplayName("Y");
  (void)AddProperty(kColor, std::string()).SetDisplayName("Color");
}

std::unique_ptr<mvvm::SessionItem> NodeItem::Clone() const
{
  return std::make_unique<NodeItem>(*this);
}

double NodeItem::GetX() const
{
  return Property<double>(kXpos);
}

void NodeItem::SetX(double value)
{
  (void)SetProperty(kXpos, value);
}

double NodeItem::GetY() const
{
  return Property<double>(kYpos);
}

void NodeItem::SetY(double value)
{
  (void)SetProperty(kYpos, value);
}

std::string NodeItem::GetBaseColor() const
{
  return Property<std::string>(kColor);
}

void NodeItem::SetBaseColor(const std::string& value)
{
  (void)SetProperty(kColor, value);
}

void NodeItem::RegisterSingleChildTag()
{
  RegisterTag(TagInfo(constants::kChildrenTag, 0, 1, {}), /*set_as_default*/ true);
}

void NodeItem::RegisterMultipleChildrenTag()
{
  RegisterTag(TagInfo::CreateUniversalTag(constants::kChildrenTag), /*set_as_default*/ true);
}

}  // namespace mvvm
