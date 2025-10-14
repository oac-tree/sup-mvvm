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

#ifndef MVVM_STANDARDITEMS_AXIS_ITEMS_H_
#define MVVM_STANDARDITEMS_AXIS_ITEMS_H_

//! @file
//! Collection of axis items for 1D and 2D data/plotting support.

#include <mvvm/model/compound_item.h>

#include <memory>
#include <vector>

namespace mvvm
{

class TextItem;

/**
 * @brief The BasicAxisItem class is a base class for all axes items.
 *
 * It has min, max defined, but nothing else.
 */
class BasicAxisItem : public CompoundItem
{
public:
  explicit BasicAxisItem(const std::string& model_type);
  ~BasicAxisItem() override = default;

  double GetMin() const;
  void SetMin(double value);

  double GetMax() const;
  void SetMax(double value);

protected:
  void RegisterMinMax();
};

/**
 * @brief The ViewportAxisItem class represents viewport axis.
 *
 * It can be a counterpart of either Qt's QAbstractAxis or QCustomPlot::QCPAxis. Intended to
 * cary title, fonts etc.
 */
class ViewportAxisItem : public BasicAxisItem
{
public:
  explicit ViewportAxisItem(const std::string& model_type = GetTypeName<ViewportAxisItem>());

  std::unique_ptr<SessionItem> Clone() const override;

  TextItem* GetTitleItem() const;

  /**
   * @brief Returns pair of lower, upper axis range.
   */
  std::pair<double, double> GetRange() const;

  /**
   * @brief Sets lower, upper range of axis to given values.
   */
  void SetRange(double lower, double upper);

  bool IsInLog() const;

  void SetInLog(bool value);
};

/**
 * @brief The BinnedAxisItem class represents an axis with arbitrary binning.
 *
 * Base class to define an axis with specific binning (fixed, variable). Used in Data1DItem and
 * Data2Ditem to store 1d and 2d data.  Doesn't carry any appearance info (e.g. axis title, label
 * size, etc) and thus not intended for direct plotting.
 */
class BinnedAxisItem : public BasicAxisItem
{
public:
  explicit BinnedAxisItem(const std::string& model_type);

  virtual std::pair<double, double> GetRange() const = 0;

  virtual std::size_t GetSize() const = 0;

  virtual std::vector<double> GetBinCenters() const = 0;
};

/**
 * @brief The FixedBinAxisItem class represents fixed bin axis.
 *
 * Defines an axis with equidistant binning.
 */
class FixedBinAxisItem : public BinnedAxisItem
{
public:
  explicit FixedBinAxisItem(const std::string& model_type = GetTypeName<FixedBinAxisItem>());

  std::unique_ptr<SessionItem> Clone() const override;

  void SetParameters(std::size_t nbins, double xmin, double xmax);

  static std::unique_ptr<FixedBinAxisItem> Create(std::size_t nbins, double xmin, double xmax);

  std::pair<double, double> GetRange() const override;

  std::size_t GetSize() const override;

  std::vector<double> GetBinCenters() const override;
};

/**
 * @brief The PointwiseAxisItem class represents a pointwise axis.
 *
 * Defines an axis via an array of points representing point coordinates.
 */
class PointwiseAxisItem : public BinnedAxisItem
{
public:
  explicit PointwiseAxisItem(const std::string& model_type = GetTypeName<PointwiseAxisItem>());

  std::unique_ptr<SessionItem> Clone() const override;

  void SetParameters(const std::vector<double>& data);

  static std::unique_ptr<PointwiseAxisItem> Create(const std::vector<double>& data);

  std::pair<double, double> GetRange() const override;

  std::size_t GetSize() const override;

  std::vector<double> GetBinCenters() const override;
};

template <>
struct item_traits<ViewportAxisItem>
{
  static constexpr std::string_view type_name() noexcept { return "ViewportAxis"; }
};

template <>
struct item_traits<FixedBinAxisItem>
{
  static constexpr std::string_view type_name() noexcept { return "FixedBinAxis"; }
};

template <>
struct item_traits<PointwiseAxisItem>
{
  static constexpr std::string_view type_name() noexcept { return "PointwiseAxis"; }
};

}  // namespace mvvm

#endif  // MVVM_STANDARDITEMS_AXIS_ITEMS_H_
