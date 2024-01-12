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

#include "data1d_item.h"

#include "axis_items.h"

#include <mvvm/interfaces/sessionmodel_interface.h>

#include <stdexcept>

namespace
{
size_t total_bin_count(mvvm::Data1DItem* item)
{
  auto axis = item->GetAxis();
  return axis ? static_cast<size_t>(axis->GetSize()) : 0;
}
}  // namespace

namespace mvvm
{

static inline const std::string kAxis = "kAxis";

Data1DItem::Data1DItem() : CompoundItem(Type)
{
  // prevent editing in widgets, since there is no corresponding editor
  AddProperty(kValues, std::vector<double>()).SetDisplayName("Values").SetEditable(false);

  AddProperty(kErrors, std::vector<double>()).SetDisplayName("Errors").SetEditable(false);

  RegisterTag(TagInfo(kAxis, 0, 1, {FixedBinAxisItem::Type, PointwiseAxisItem::Type}), true);
}

std::unique_ptr<SessionItem> Data1DItem::Clone(bool make_unique_id) const
{
  return std::make_unique<Data1DItem>(*this, make_unique_id);
}

//! Returns coordinates of bin centers.

std::vector<double> Data1DItem::GetBinCenters() const
{
  auto axis = GetItem<BinnedAxisItem>(kAxis);
  return axis ? axis->GetBinCenters() : std::vector<double>{};
}

//! Sets internal data buffer to given data. If size of axis doesn't match the size of the data,
//! exception will be thrown.

void Data1DItem::SetValues(const std::vector<double>& data)
{
  if (total_bin_count(this) != data.size())
  {
    throw RuntimeException("Error in Data1DItem: data doesn't match size of axis");
  }

  SetProperty(kValues, data);
}

//! Returns values stored in bins.

std::vector<double> Data1DItem::GetValues() const
{
  return Property<std::vector<double>>(kValues);
}

//! Sets errors on values in bins.

void Data1DItem::SetErrors(const std::vector<double>& errors)
{
  if (total_bin_count(this) != errors.size())
  {
    throw RuntimeException("Error in Data1DItem: data doesn't match size of axis");
  }

  SetProperty(kErrors, errors);
}

//! Returns value errors stored in bins.

std::vector<double> Data1DItem::GetErrors() const
{
  return Property<std::vector<double>>(kErrors);
}

BinnedAxisItem* Data1DItem::GetAxis() const
{
  return GetItem<BinnedAxisItem>({kAxis, 0});
}

void Data1DItem::SetAxis(std::unique_ptr<BinnedAxisItem> axis)
{
  // we disable possibility to re-create axis to facilitate undo/redo
  if (GetAxis())
  {
    throw RuntimeException("Error in Data1DItem: axis was already set");
  }

  GetModel() ? GetModel()->InsertItem(std::move(axis), this, {kAxis, 0})
             : InsertItem(std::move(axis), {kAxis, 0});

  SetValues(std::vector<double>(GetAxis()->GetSize(), 0.0));
}

}  // namespace mvvm
