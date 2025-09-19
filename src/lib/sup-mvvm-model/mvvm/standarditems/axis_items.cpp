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

#include "axis_items.h"

#include "plottable_items.h"

namespace
{
const double kDefaultAxisMin = 0.0;
const double kDefaultAxisMax = 1.0;
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
  (void)SetProperty(kMin, value);
}

double BasicAxisItem::GetMax() const
{
  return Property<double>(kMax);
}

void BasicAxisItem::SetMax(double value)
{
  (void)SetProperty(kMax, value);
}

void BasicAxisItem::RegisterMinMax()
{
  (void)AddProperty(kMin, kDefaultAxisMin).SetDisplayName("Min");
  (void)AddProperty(kMax, kDefaultAxisMax).SetDisplayName("Max");
}

// --- ViewportAxisItem ------------------------------------------------------

static inline const std::string kTitle = "kTitle";
static inline const std::string kIsLog = "kIsLog";

ViewportAxisItem::ViewportAxisItem(const std::string& model_type) : BasicAxisItem(model_type)
{
  (void)AddProperty<TextItem>(kTitle).SetDisplayName("Title");
  RegisterMinMax();
  (void)AddProperty(kIsLog, false).SetDisplayName("log10");
}

std::unique_ptr<SessionItem> ViewportAxisItem::Clone() const
{
  return std::make_unique<ViewportAxisItem>(*this);
}

TextItem* ViewportAxisItem::GetTitleItem() const
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
  (void)SetProperty(kIsLog, value);
}

// --- BinnedAxisItem ------------------------------------------------------

BinnedAxisItem::BinnedAxisItem(const std::string& model_type) : BasicAxisItem(model_type) {}

static inline const std::string kNbins = "kNbins";

// --- FixedBinAxisItem ------------------------------------------------------

FixedBinAxisItem::FixedBinAxisItem(const std::string& model_type) : BinnedAxisItem(model_type)
{
  (void)AddProperty(kNbins, std::size_t{1}).SetDisplayName("Nbins");
  RegisterMinMax();
}

std::unique_ptr<SessionItem> FixedBinAxisItem::Clone() const
{
  return std::make_unique<FixedBinAxisItem>(*this);
}

void FixedBinAxisItem::SetParameters(std::size_t nbins, double xmin, double xmax)
{
  (void)SetProperty(kNbins, nbins);
  SetMin(xmin);
  SetMax(xmax);
}

std::unique_ptr<FixedBinAxisItem> FixedBinAxisItem::Create(std::size_t nbins, double xmin, double xmax)
{
  auto result = std::make_unique<FixedBinAxisItem>();
  result->SetParameters(nbins, xmin, xmax);
  return result;
}

std::pair<double, double> FixedBinAxisItem::GetRange() const
{
  return std::make_pair(GetMin(), GetMax());
}

std::size_t FixedBinAxisItem::GetSize() const
{
  return Property<std::size_t>(kNbins);
}

std::vector<double> FixedBinAxisItem::GetBinCenters() const
{
  std::vector<double> result;
  const auto nbins = GetSize();
  const double step = (GetMax() - GetMin()) / nbins;

  result.resize(nbins, 0.0);
  for (std::size_t i = 0; i < nbins; ++i)
  {
    result[i] = GetMin() + step * (static_cast<double>(i) + 0.5);
  }

  return result;
}

// --- PointwiseAxisItem ------------------------------------------------------

PointwiseAxisItem::PointwiseAxisItem(const std::string& model_type) : BinnedAxisItem(model_type)
{
  // vector of points matching default xmin, xmax
  (void)SetData(std::vector<double>{kDefaultAxisMin, kDefaultAxisMax});
  (void)SetEditable(false);  // prevent editing in widgets, since there is no corresponding editor
}

std::unique_ptr<SessionItem> PointwiseAxisItem::Clone() const
{
  return std::make_unique<PointwiseAxisItem>(*this);
}

void PointwiseAxisItem::SetParameters(const std::vector<double>& data)
{
  (void)SetData(data);
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
  return GetBinCenters().empty() ? std::make_pair(kDefaultAxisMin, kDefaultAxisMax)
                                 : std::make_pair(data.front(), data.back());
}

std::size_t PointwiseAxisItem::GetSize() const
{
  return GetBinCenters().size();
}

std::vector<double> PointwiseAxisItem::GetBinCenters() const
{
  return Data<std::vector<double>>();
}

}  // namespace mvvm
