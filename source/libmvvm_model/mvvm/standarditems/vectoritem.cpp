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

#include "mvvm/standarditems/vectoritem.h"

#include "mvvm/signals/itemconnectutils.h"

#include <sstream>

namespace mvvm
{
VectorItem::VectorItem() : CompoundItem(Type)
{
  AddProperty(kX, 0.0)->SetDisplayName("X");
  AddProperty(kY, 0.0)->SetDisplayName("Y");
  AddProperty(kZ, 0.0)->SetDisplayName("Z");

  SetEditable(false);

  UpdateLabel();
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
  auto on_property_change = [this](SessionItem*, const std::string&) { UpdateLabel(); };
  connect::OnPropertyChanged(this, on_property_change);
}

void VectorItem::UpdateLabel()
{
  std::ostringstream ostr;
  ostr << "(" << X() << ", " << Y() << ", " << Z() << ")";
  SetData(ostr.str(), DataRole::kData);
}

}  // namespace mvvm
