/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Authors       : Gennady Pospelov (IO)
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

using namespace ModelView;

VectorItem::VectorItem() : CompoundItem(Type)
{
  addProperty(P_X, 0.0)->setDisplayName("X");
  addProperty(P_Y, 0.0)->setDisplayName("Y");
  addProperty(P_Z, 0.0)->setDisplayName("Z");

  setEditable(false);

  update_label();
}

double VectorItem::x() const
{
  return property<double>(P_X);
}

void VectorItem::setX(double value)
{
  setProperty(P_X, value);
}

double VectorItem::y() const
{
  return property<double>(P_Y);
}

void VectorItem::setY(double value)
{
  setProperty(P_Y, value);
}

double VectorItem::z() const
{
  return property<double>(P_Z);
}

void VectorItem::setZ(double value)
{
  setProperty(P_Z, value);
}

void VectorItem::update_label()
{
  std::ostringstream ostr;
  ostr << "(" << x() << ", " << y() << ", " << z() << ")";
  setData(ostr.str(), DataRole::kData, /*direct*/ true);
}
