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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "vector_item.h"

#include <mvvm/signals/item_connect.h>

#include <sstream>

namespace mvvm
{
VectorItem::VectorItem() : CompoundItem(GetStaticType())
{
  AddProperty(kX, 0.0).SetDisplayName("X");
  AddProperty(kY, 0.0).SetDisplayName("Y");
  AddProperty(kZ, 0.0).SetDisplayName("Z");

  SetEditable(false);

  UpdateLabel();
}

std::string VectorItem::GetStaticType()
{
  return "VectorItem";
}

std::unique_ptr<SessionItem> VectorItem::Clone() const
{
  return std::make_unique<VectorItem>(*this);
}

double VectorItem::X() const
{
  return Property<double>(kX);
}

void VectorItem::SetX(double value)
{
  SetProperty(kX, value);
}

double VectorItem::Y() const
{
  return Property<double>(kY);
}

void VectorItem::SetY(double value)
{
  SetProperty(kY, value);
}

double VectorItem::Z() const
{
  return Property<double>(kZ);
}

void VectorItem::SetZ(double value)
{
  SetProperty(kZ, value);
}

void VectorItem::SetXYZ(double x, double y, double z)
{
  SetX(x);
  SetY(y);
  SetZ(z);
}

void VectorItem::Activate()
{
  auto on_property_change = [this](const PropertyChangedEvent&) { UpdateLabel(); };
  connect::Connect<PropertyChangedEvent>(this, on_property_change, GetSlot());
}

void VectorItem::UpdateLabel()
{
  std::ostringstream ostr;
  ostr << "(" << X() << ", " << Y() << ", " << Z() << ")";
  SetData(ostr.str(), DataRole::kData);
}

}  // namespace mvvm
