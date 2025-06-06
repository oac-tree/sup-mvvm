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

#include "line_series_data_item.h"

#include <mvvm/model/item_constants.h>
#include <mvvm/model/item_utils.h>
#include <mvvm/standarditems/point_item.h>

namespace mvvm
{

LineSeriesDataItem::LineSeriesDataItem() : CompoundItem(GetStaticType())
{
  RegisterTag(TagInfo::CreateUniversalTag(constants::kChildrenTag), /*set_as_default*/ true);
}

std::string LineSeriesDataItem::GetStaticType()
{
  return "LineSeriesData";
}

std::unique_ptr<SessionItem> LineSeriesDataItem::Clone() const
{
  return std::make_unique<LineSeriesDataItem>(*this);
}

int LineSeriesDataItem::GetPointCount() const
{
  return GetItemCount(constants::kChildrenTag);
}

std::vector<std::pair<double, double> > LineSeriesDataItem::GetWaveform() const
{
  std::vector<std::pair<double, double> > result;
  for (const auto* point : GetItems<PointItem>(constants::kChildrenTag))
  {
    result.emplace_back(point->GetX(), point->GetY());
  }

  return result;
}

void LineSeriesDataItem::SetWaveform(const std::vector<std::pair<double, double> >& data)
{
  Clear();
  for (auto [x, y] : data)
  {
    auto point = std::make_unique<PointItem>();
    point->SetX(x);
    point->SetY(y);
    utils::InsertItem(std::move(point), this, TagIndex::Append());
  }
}

void LineSeriesDataItem::Clear()
{
  for (auto* child : GetPoints())
  {
    // using universal function to provide notifications, if possible
    utils::RemoveItem(*child);
  }
}

std::pair<double, double> LineSeriesDataItem::GetPointCoordinates(int index) const
{
  return GetPoint(index)->GetPointCoordinates();
}

void LineSeriesDataItem::SetPointCoordinates(int index,
                                             const std::pair<double, double>& coordinates)
{
  GetPoint(index)->SetPointCoordinates(coordinates);
}

void LineSeriesDataItem::RemovePoint(int index)
{
  auto point = GetPoint(index);
  utils::RemoveItem(*point);
}

void LineSeriesDataItem::InsertPoint(int index, const std::pair<double, double>& coordinates)
{
  auto point = std::make_unique<PointItem>();
  point->SetX(coordinates.first);
  point->SetY(coordinates.second);
  utils::InsertItem(std::move(point), this, TagIndex::Default(index));
}

void LineSeriesDataItem::ValidateIndex(int index) const
{
  auto points = GetPoints();
  if (index < 0 || static_cast<size_t>(index) >= points.size())
  {
    throw RuntimeException("Index [" + std::to_string(index) + "] doesn't match number of points ["
                           + std::to_string(points.size()) + "]");
  }
}

std::vector<PointItem*> LineSeriesDataItem::GetPoints() const
{
  return GetItems<PointItem>(constants::kChildrenTag);
}

PointItem* LineSeriesDataItem::GetPoint(int index) const
{
  ValidateIndex(index);
  return GetPoints().at(index);
}

}  // namespace mvvm
