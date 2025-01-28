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

#include "point_item.h"

#include <mvvm/model/item_constants.h>

namespace mvvm
{

PointItem::PointItem() : CompoundItem(GetStaticType())
{
  AddProperty(constants::kX, 0.0).SetDisplayName("X");
  AddProperty(constants::kY, 0.0).SetDisplayName("Y");
}

std::string PointItem::GetStaticType()
{
  return "Point";
}

std::unique_ptr<SessionItem> PointItem::Clone() const
{
  return std::make_unique<PointItem>(*this);
}

double PointItem::GetX() const
{
  return Property<double>(constants::kX);
}

void PointItem::SetX(double value)
{
  SetProperty(constants::kX, value);
}

double PointItem::GetY() const
{
  return Property<double>(constants::kY);
}

void PointItem::SetY(double value)
{
  SetProperty(constants::kY, value);
}

std::pair<double, double> PointItem::GetPointCoordinates() const
{
  return std::make_pair(GetX(), GetY());
}

void PointItem::SetPointCoordinates(const std::pair<double, double> &coordinates)
{
  SetX(coordinates.first);
  SetY(coordinates.second);
}

}  // namespace mvvm
