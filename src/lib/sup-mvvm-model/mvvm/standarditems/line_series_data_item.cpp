/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "line_series_data_item.h"

#include <mvvm/model/item_constants.h>
#include <mvvm/model/item_utils.h>
#include <mvvm/standarditems/point_item.h>

namespace mvvm
{

LineSeriesDataItem::LineSeriesDataItem() : CompoundItem(Type)
{
  RegisterTag(TagInfo::CreateUniversalTag(constants::kChildrenTag), /*set_as_default*/ true);
}

std::unique_ptr<SessionItem> LineSeriesDataItem::Clone(bool make_unique_id) const
{
  return std::make_unique<LineSeriesDataItem>(*this, make_unique_id);
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
  for (auto* child : GetItems<PointItem>(constants::kChildrenTag))
  {
    // using universal function to provide notifications, if possible
    utils::RemoveItem(*child);
  }
}

}  // namespace mvvm
