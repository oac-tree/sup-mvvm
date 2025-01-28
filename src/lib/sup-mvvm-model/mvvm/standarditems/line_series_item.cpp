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

#include "line_series_item.h"

#include "line_series_data_item.h"
#include "waveform_helper.h"

#include <mvvm/model/item_constants.h>
#include <mvvm/standarditems/linked_item.h>
#include <mvvm/standarditems/plottable_items.h>

namespace
{
const int kDefaultLineWidth = 2;
const std::string kDefaultLineColor("#209fdf");  // between royal blue and steel blue
}  // namespace

namespace mvvm
{

LineSeriesItem::LineSeriesItem() : CompoundItem(GetStaticType())
{
  AddProperty<mvvm::LinkedItem>(constants::kLink).SetDisplayName("Link").SetVisible(false);
  AddProperty(kOffset, 0.0).SetDisplayName("Offset");

  auto &pen = AddProperty<mvvm::PenItem>(constants::kPen);
  pen.SetDisplayName("Pen");
  pen.SetWidth(kDefaultLineWidth);
  pen.SetNamedColor(kDefaultLineColor);

  AddProperty(constants::kDisplayed, true).SetDisplayName("Displayed");
}

std::string LineSeriesItem::GetStaticType()
{
  return "LineSeries";
}

std::unique_ptr<SessionItem> LineSeriesItem::Clone() const
{
  return std::make_unique<LineSeriesItem>(*this);
}

void LineSeriesItem::SetDataItem(LineSeriesDataItem *item)
{
  GetItem<mvvm::LinkedItem>(constants::kLink)->SetLink(item);
}

const LineSeriesDataItem *LineSeriesItem::GetDataItem() const
{
  return GetItem<mvvm::LinkedItem>(constants::kLink)->Get<LineSeriesDataItem>();
}

LineSeriesDataItem *LineSeriesItem::GetDataItem()
{
  return const_cast<LineSeriesDataItem *>(static_cast<const LineSeriesItem *>(this)->GetDataItem());
}

double LineSeriesItem::GetXOffset() const
{
  return Property<double>(kOffset);
}

void LineSeriesItem::SetXOffset(double value)
{
  SetProperty<double>(kOffset, value);
}

std::string LineSeriesItem::GetNamedColor() const
{
  return GetPenItem()->GetNamedColor();
}

void LineSeriesItem::SetNamedColor(const std::string &named_color)
{
  GetPenItem()->SetNamedColor(named_color);
}

mvvm::PenItem *LineSeriesItem::GetPenItem() const
{
  return GetItem<mvvm::PenItem>(constants::kPen);
}

std::vector<double> LineSeriesItem::GetBinCenters() const
{
  // retrieving x-coordinates and shifting them by the offset value
  auto result = GetPairOfVectors(GetDataItem()->GetWaveform()).first;
  std::transform(result.begin(), result.end(), result.begin(),
                 [this](auto element) { return element += GetXOffset(); });
  return result;
}

std::vector<double> LineSeriesItem::GetValues() const
{
  return GetPairOfVectors(GetDataItem()->GetWaveform()).second;
}

bool LineSeriesItem::IsDisplayed() const
{
  return Property<bool>(constants::kDisplayed);
}

void LineSeriesItem::SetDisplayed(bool value)
{
  SetProperty(constants::kDisplayed, value);
}

}  // namespace mvvm
