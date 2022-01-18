// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#include "mvvm/standarditems/data1ditem.h"

#include "mvvm/standarditems/axisitems.h"

#include <stdexcept>

using namespace mvvm;

namespace
{
size_t total_bin_count(Data1DItem* item)
{
  auto axis = item->GetItem<BinnedAxisItem>(Data1DItem::T_AXIS);
  return axis ? static_cast<size_t>(axis->GetSize()) : 0;
}
}  // namespace

Data1DItem::Data1DItem() : CompoundItem(Type)
{
  // prevent editing in widgets, since there is no corresponding editor
  AddProperty(P_VALUES, std::vector<double>())->SetDisplayName("Values")->SetEditable(false);

  AddProperty(P_ERRORS, std::vector<double>())->SetDisplayName("Errors")->SetEditable(false);

  RegisterTag(TagInfo(T_AXIS, 0, 1, {FixedBinAxisItem::Type, PointwiseAxisItem::Type}), true);
}

//! Returns coordinates of bin centers.

std::vector<double> Data1DItem::binCenters() const
{
  auto axis = GetItem<BinnedAxisItem>(T_AXIS);
  return axis ? axis->GetBinCenters() : std::vector<double>{};
}

//! Sets internal data buffer to given data. If size of axis doesn't match the size of the data,
//! exception will be thrown.

void Data1DItem::setValues(const std::vector<double>& data)
{
  if (total_bin_count(this) != data.size())
    throw std::runtime_error("Data1DItem::setValues() -> Data doesn't match size of axis");

  SetProperty(P_VALUES, data);
}

//! Returns values stored in bins.

std::vector<double> Data1DItem::binValues() const
{
  return Property<std::vector<double>>(P_VALUES);
}

//! Sets errors on values in bins.

void Data1DItem::setErrors(const std::vector<double>& errors)
{
  if (total_bin_count(this) != errors.size())
    throw std::runtime_error("Data1DItem::setErrors() -> Data doesn't match size of axis");

  SetProperty(P_ERRORS, errors);
}

//! Returns value errors stored in bins.

std::vector<double> Data1DItem::binErrors() const
{
  return Property<std::vector<double>>(P_ERRORS);
}
