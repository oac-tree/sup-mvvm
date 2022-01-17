// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#include "mvvm/standarditems/axisitems.h"

#include "mvvm/standarditems/plottableitems.h"

namespace
{
const double default_axis_min = 0.0;
const double default_axis_max = 1.0;
}  // namespace

using namespace mvvm;

BasicAxisItem::BasicAxisItem(const std::string& model_type) : CompoundItem(model_type) {}

void BasicAxisItem::register_min_max()
{
  AddProperty(P_MIN, default_axis_min)->SetDisplayName("Min");
  AddProperty(P_MAX, default_axis_max)->SetDisplayName("Max");
}

// --- ViewportAxisItem ------------------------------------------------------

ViewportAxisItem::ViewportAxisItem(const std::string& model_type) : BasicAxisItem(model_type)
{
  AddProperty<TextItem>(P_TITLE)->SetDisplayName("Title");
  register_min_max();
  AddProperty(P_IS_LOG, false)->SetDisplayName("log10");
}

//! Returns pair of lower, upper axis range.

std::pair<double, double> ViewportAxisItem::range() const
{
  return std::make_pair(Property<double>(P_MIN), Property<double>(P_MAX));
}

//! Sets lower, upper range of axis to given values.

void ViewportAxisItem::set_range(double lower, double upper)
{
  SetProperty(P_MIN, lower);
  SetProperty(P_MAX, upper);
}

bool ViewportAxisItem::is_in_log() const
{
  return Property<bool>(P_IS_LOG);
}

// --- BinnedAxisItem ------------------------------------------------------

BinnedAxisItem::BinnedAxisItem(const std::string& model_type) : BasicAxisItem(model_type) {}

// --- FixedBinAxisItem ------------------------------------------------------

FixedBinAxisItem::FixedBinAxisItem(const std::string& model_type) : BinnedAxisItem(model_type)
{
  AddProperty(P_NBINS, 1)->SetDisplayName("Nbins");
  register_min_max();
}

void FixedBinAxisItem::setParameters(int nbins, double xmin, double xmax)
{
  SetProperty(P_NBINS, nbins);
  SetProperty(P_MIN, xmin);
  SetProperty(P_MAX, xmax);
}

std::unique_ptr<FixedBinAxisItem> FixedBinAxisItem::create(int nbins, double xmin, double xmax)
{
  auto result = std::make_unique<FixedBinAxisItem>();
  result->setParameters(nbins, xmin, xmax);
  return result;
}

std::pair<double, double> FixedBinAxisItem::range() const
{
  return std::make_pair(Property<double>(P_MIN), Property<double>(P_MAX));
}

int FixedBinAxisItem::size() const
{
  return Property<int>(P_NBINS);
}

std::vector<double> FixedBinAxisItem::binCenters() const
{
  std::vector<double> result;
  int nbins = Property<int>(P_NBINS);
  double start = Property<double>(P_MIN);
  double end = Property<double>(P_MAX);
  double step = (end - start) / nbins;

  result.resize(static_cast<size_t>(nbins), 0.0);
  for (size_t i = 0; i < static_cast<size_t>(nbins); ++i)
    result[i] = start + step * (i + 0.5);

  return result;
}

// --- PointwiseAxisItem ------------------------------------------------------

PointwiseAxisItem::PointwiseAxisItem(const std::string& model_type) : BinnedAxisItem(model_type)
{
  // vector of points matching default xmin, xmax
  SetData(std::vector<double>{default_axis_min, default_axis_max});
  SetEditable(false);  // prevent editing in widgets, since there is no corresponding editor
}

void PointwiseAxisItem::setParameters(const std::vector<double>& data)
{
  SetData(data);
}

std::unique_ptr<PointwiseAxisItem> PointwiseAxisItem::create(const std::vector<double>& data)
{
  auto result = std::make_unique<PointwiseAxisItem>();
  result->setParameters(data);
  return result;
}

std::pair<double, double> PointwiseAxisItem::range() const
{
  auto data = binCenters();
  return binCenters().empty() ? std::make_pair(default_axis_min, default_axis_max)
                              : std::make_pair(data.front(), data.back());
}

int PointwiseAxisItem::size() const
{
  return binCenters().size();
}

std::vector<double> PointwiseAxisItem::binCenters() const
{
  return Data<std::vector<double>>();
}
