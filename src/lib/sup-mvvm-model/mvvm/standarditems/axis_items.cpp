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

#include "axis_items.h"

#include "plottable_items.h"

namespace
{
const double default_axis_min = 0.0;
const double default_axis_max = 1.0;
}  // namespace

namespace mvvm
{

static inline const std::string kMin = "kMin";
static inline const std::string kMax = "kMax";

BasicAxisItem::BasicAxisItem(const std::string& model_type) : CompoundItem(model_type) {}

double BasicAxisItem::GetMin() const
{
  return Property<double>(kMin);
}

void BasicAxisItem::SetMin(double value)
{
  return SetProperty(kMin, value);
}

double BasicAxisItem::GetMax() const
{
  return Property<double>(kMax);
}

void BasicAxisItem::SetMax(double value)
{
  return SetProperty(kMax, value);
}

void BasicAxisItem::RegisterMinMax()
{
  AddProperty(kMin, default_axis_min).SetDisplayName("Min");
  AddProperty(kMax, default_axis_max).SetDisplayName("Max");
}

// --- ViewportAxisItem ------------------------------------------------------

static inline const std::string kTitle = "kTitle";
static inline const std::string kIsLog = "kIsLog";

ViewportAxisItem::ViewportAxisItem(const std::string& model_type) : BasicAxisItem(model_type)
{
  AddProperty<TextItem>(kTitle).SetDisplayName("Title");
  RegisterMinMax();
  AddProperty(kIsLog, false).SetDisplayName("log10");
}

std::unique_ptr<SessionItem> ViewportAxisItem::Clone(bool make_unique_id) const
{
  return std::make_unique<ViewportAxisItem>(*this, make_unique_id);
}

TextItem* ViewportAxisItem::GetTitle() const
{
  return GetItem<TextItem>(kTitle);
}

std::pair<double, double> ViewportAxisItem::GetRange() const
{
  return std::make_pair(GetMin(), GetMax());
}

void ViewportAxisItem::SetRange(double lower, double upper)
{
  SetMin(lower);
  SetMax(upper);
}

bool ViewportAxisItem::IsInLog() const
{
  return Property<bool>(kIsLog);
}

void ViewportAxisItem::SetInLog(bool value)
{
  SetProperty(kIsLog, value);
}

// --- BinnedAxisItem ------------------------------------------------------

BinnedAxisItem::BinnedAxisItem(const std::string& model_type) : BasicAxisItem(model_type) {}

static inline const std::string kNbins = "kNbins";

// --- FixedBinAxisItem ------------------------------------------------------

FixedBinAxisItem::FixedBinAxisItem(const std::string& model_type) : BinnedAxisItem(model_type)
{
  AddProperty(kNbins, 1).SetDisplayName("Nbins");
  RegisterMinMax();
}

std::unique_ptr<SessionItem> FixedBinAxisItem::Clone(bool make_unique_id) const
{
  return std::make_unique<FixedBinAxisItem>(*this, make_unique_id);
}

void FixedBinAxisItem::SetParameters(int nbins, double xmin, double xmax)
{
  SetProperty(kNbins, nbins);
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
  return std::make_pair(GetMin(), GetMax());
}

int FixedBinAxisItem::GetSize() const
{
  return Property<int>(kNbins);
}

std::vector<double> FixedBinAxisItem::GetBinCenters() const
{
  std::vector<double> result;
  const int nbins = GetSize();
  const double step = (GetMax() - GetMin()) / nbins;

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

std::unique_ptr<SessionItem> PointwiseAxisItem::Clone(bool make_unique_id) const
{
  return std::make_unique<PointwiseAxisItem>(*this, make_unique_id);
}

void PointwiseAxisItem::SetParameters(const std::vector<double>& data)
{
  SetData(data);
}

std::unique_ptr<PointwiseAxisItem> PointwiseAxisItem::Create(const std::vector<double>& data)
{
  auto result = std::make_unique<PointwiseAxisItem>();
  result->SetParameters(data);
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
  return static_cast<int>(GetBinCenters().size());
}

std::vector<double> PointwiseAxisItem::GetBinCenters() const
{
  return Data<std::vector<double>>();
}

}  // namespace mvvm
