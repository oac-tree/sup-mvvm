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

namespace mvvm
{

namespace
{
constexpr std::string_view kXpos{"kXpos"};
constexpr std::string_view kYpos{"kYpos"};
constexpr std::string_view kColor{"kColor"};
}  // namespace

NodeItem::NodeItem() : CompoundItem(GetTypeName<NodeItem>())
{
  AddProperty(std::string(kXpos), 0.0).SetDisplayName("X");
  AddProperty(std::string(kYpos), 0.0).SetDisplayName("Y");
  AddProperty(std::string(kColor), std::string()).SetDisplayName("Color");
}

std::unique_ptr<mvvm::SessionItem> NodeItem::Clone() const
{
  return std::make_unique<NodeItem>(*this);
}

double NodeItem::GetX() const
{
  return Property<double>(std::string(kXpos));
}

void NodeItem::SetX(double value)
{
  SetProperty(std::string(kXpos), value);
}

double NodeItem::GetY() const
{
  return Property<double>(std::string(kYpos));
}

void NodeItem::SetY(double value)
{
  SetProperty(std::string(kYpos), value);
}

std::string NodeItem::GetBaseColor() const
{
  return Property<std::string>(std::string(kColor));
}

void NodeItem::SetBaseColor(const std::string &value)
{
  SetProperty(std::string(kColor), value);
}

}  // namespace mvvm
