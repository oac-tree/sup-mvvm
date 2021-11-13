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

#include <sstream>

namespace ModelView
{
VectorItem::VectorItem() : CompoundItem(Type)
{
  AddProperty(P_X, 0.0)->SetDisplayName("X");
  AddProperty(P_Y, 0.0)->SetDisplayName("Y");
  AddProperty(P_Z, 0.0)->SetDisplayName("Z");

  SetEditable(false);

  UpdateLabel();
}

double VectorItem::X() const
{
  return Property<double>(P_X);
}

void VectorItem::SetX(double value)
{
  SetProperty(P_X, value);
}

double VectorItem::Y() const
{
  return Property<double>(P_Y);
}

void VectorItem::SetY(double value)
{
  SetProperty(P_Y, value);
}

double VectorItem::Z() const
{
  return Property<double>(P_Z);
}

void VectorItem::SetZ(double value)
{
  SetProperty(P_Z, value);
}

void VectorItem::UpdateLabel()
{
  std::ostringstream ostr;
  ostr << "(" << X() << ", " << Y() << ", " << Z() << ")";
  SetData(ostr.str(), DataRole::kData);
}

}  // namespace ModelView
