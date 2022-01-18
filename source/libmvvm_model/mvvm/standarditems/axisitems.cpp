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

namespace mvvm
{

static inline const std::string P_MIN = "P_MIN";
static inline const std::string P_MAX = "P_MAX";

BasicAxisItem::BasicAxisItem(const std::string& model_type) : CompoundItem(model_type) {}

double BasicAxisItem::GetMin() const
{
  return Property<double>(P_MIN);
}

void BasicAxisItem::SetMin(double value)
{
  return SetProperty(P_MIN, value);
}

double BasicAxisItem::GetMax() const
{
  return Property<double>(P_MAX);
}

void BasicAxisItem::SetMax(double value)
{
  return SetProperty(P_MAX, value);
}

void BasicAxisItem::RegisterMinMax()
{
  AddProperty(P_MIN, default_axis_min)->SetDisplayName("Min");
  AddProperty(P_MAX, default_axis_max)->SetDisplayName("Max");
}

// --- ViewportAxisItem ------------------------------------------------------

static inline const std::string P_TITLE = "P_TITLE";
static inline const std::string P_IS_LOG = "P_IS_LOG";

ViewportAxisItem::ViewportAxisItem(const std::string& model_type) : BasicAxisItem(model_type)
{
  AddProperty<TextItem>(P_TITLE)->SetDisplayName("Title");
  RegisterMinMax();
  AddProperty(P_IS_LOG, false)->SetDisplayName("log10");
}

TextItem* ViewportAxisItem::GetTitle() const
{
  return GetItem<TextItem>(P_TITLE);
}

//! Returns pair of lower, upper axis range.

std::pair<double, double> ViewportAxisItem::GetRange() const
{
  return std::make_pair(GetMin(), GetMax());
}

//! Sets lower, upper range of axis to given values.

void ViewportAxisItem::SetRange(double lower, double upper)
{
  SetMin(lower);
  SetMax(upper);
}

bool ViewportAxisItem::IsInLog() const
{
  return Property<bool>(P_IS_LOG);
}

void ViewportAxisItem::SetInLog(bool value)
{
  SetProperty(P_IS_LOG, value);
}

// --- BinnedAxisItem ------------------------------------------------------

BinnedAxisItem::BinnedAxisItem(const std::string& model_type) : BasicAxisItem(model_type) {}

// --- FixedBinAxisItem ------------------------------------------------------

FixedBinAxisItem::FixedBinAxisItem(const std::string& model_type) : BinnedAxisItem(model_type)
{
  AddProperty(P_NBINS, 1)->SetDisplayName("Nbins");
  RegisterMinMax();
}

void FixedBinAxisItem::SetParameters(int nbins, double xmin, double xmax)
{
  SetProperty(P_NBINS, nbins);
  SetMin(xmin);
  SetMax(xmax);
}

std::unique_ptr<FixedBinAxisItem> FixedBinAxisItem::Create(int nbins, double xmin, double xmax)
{
  auto result = std::make_unique<FixedBinAxisItem>();
  result->SetParameters(nbins, xmin, xmax);
  return result;
}

std::pair<double, double> FixedBinAxisItem::GetRange() const
{
  return std::make_pair(Property<double>(P_MIN), Property<double>(P_MAX));
}

int FixedBinAxisItem::GetSize() const
{
  return Property<int>(P_NBINS);
}

std::vector<double> FixedBinAxisItem::GetBinCenters() const
{
  std::vector<double> result;
  int nbins = GetSize();
  double step = (GetMax() - GetMin()) / nbins;

  result.resize(static_cast<size_t>(nbins), 0.0);
  for (int i = 0; i < nbins; ++i)
  {
    result[i] = GetMin() + step * (i + 0.5);
  }

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

std::pair<double, double> PointwiseAxisItem::GetRange() const
{
  auto data = GetBinCenters();
  return GetBinCenters().empty() ? std::make_pair(default_axis_min, default_axis_max)
                                 : std::make_pair(data.front(), data.back());
}

int PointwiseAxisItem::GetSize() const
{
  return GetBinCenters().size();
}

std::vector<double> PointwiseAxisItem::GetBinCenters() const
{
  return Data<std::vector<double>>();
}

}  // namespace mvvm
