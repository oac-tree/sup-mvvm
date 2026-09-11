/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "data1d_item.h"

#include "axis_items.h"

#include <mvvm/model/make_clone.h>

namespace mvvm
{

namespace
{

size_t GetTotalBinCount(const mvvm::Data1DItem& item)
{
  auto axis = item.GetAxis();
  return (axis != nullptr) ? axis->GetSize() : 0;
}

}  // namespace

static inline const std::string_view kAxis = "kAxis";

Data1DItem::Data1DItem() : CompoundItem(GetTypeName<Data1DItem>())
{
  // prevent editing in widgets, since there is no corresponding editor
  (void)AddProperty(kValues, std::vector<double>()).SetDisplayName("Values").SetEditable(false);

  (void)AddProperty(kErrors, std::vector<double>()).SetDisplayName("Errors").SetEditable(false);

  RegisterTag(TagInfo(std::string(kAxis), 0, 1,
                      {GetTypeName<FixedBinAxisItem>(), GetTypeName<PointwiseAxisItem>()}),
              true);
}

std::unique_ptr<SessionItem> Data1DItem::Clone() const
{
  return MakeClone(*this);
}

//! Returns coordinates of bin centers.

std::vector<double> Data1DItem::GetBinCenters() const
{
  const auto* axis = FindItem<BinnedAxisItem>(TagIndex(kAxis));
  return (axis != nullptr) ? axis->GetBinCenters() : std::vector<double>{};
}

//! Sets internal data buffer to given data. If size of axis doesn't match the size of the data,
//! exception will be thrown.

void Data1DItem::SetValues(const std::vector<double>& data)
{
  if (GetTotalBinCount(*this) != data.size())
  {
    throw RuntimeException("Error in Data1DItem: data doesn't match size of axis");
  }

  (void)SetProperty(kValues, data);
}

//! Returns values stored in bins.

std::vector<double> Data1DItem::GetValues() const
{
  return Property<std::vector<double>>(kValues);
}

//! Sets errors on values in bins.

void Data1DItem::SetErrors(const std::vector<double>& errors)
{
  if (GetTotalBinCount(*this) != errors.size())
  {
    throw RuntimeException("Error in Data1DItem: data doesn't match size of axis");
  }

  (void)SetProperty(kErrors, errors);
}

//! Returns value errors stored in bins.

std::vector<double> Data1DItem::GetErrors() const
{
  return Property<std::vector<double>>(kErrors);
}

const BinnedAxisItem* Data1DItem::GetAxis() const
{
  return FindItem<BinnedAxisItem>(kAxis);
}

BinnedAxisItem* Data1DItem::GetAxis()
{
  return FindItem<BinnedAxisItem>(kAxis);
}

void Data1DItem::SetAxis(std::unique_ptr<BinnedAxisItem> axis)
{
  // we disable possibility to re-create axis to facilitate undo/redo
  if (GetAxis() != nullptr)
  {
    throw RuntimeException("Error in Data1DItem: axis was already set");
  }

  (void)InsertItem(std::move(axis), {kAxis, 0});

  SetValues(std::vector<double>(GetAxis()->GetSize(), 0.0));
}

}  // namespace mvvm
