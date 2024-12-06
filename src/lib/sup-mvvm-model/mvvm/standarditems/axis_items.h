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
class MVVM_MODEL_EXPORT BasicAxisItem : public CompoundItem
{
public:
  static inline const std::string kMin = "kMin";
  static inline const std::string kMax = "kMax";

  explicit BasicAxisItem(const std::string& model_type);

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
class MVVM_MODEL_EXPORT ViewportAxisItem : public BasicAxisItem
{
public:
  static inline const std::string Type = "ViewportAxis";
  static inline const std::string kIsLog = "kIsLog";

  explicit ViewportAxisItem(const std::string& model_type = Type);

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
class MVVM_MODEL_EXPORT BinnedAxisItem : public BasicAxisItem
{
public:
  explicit BinnedAxisItem(const std::string& model_type);

  virtual std::pair<double, double> GetRange() const = 0;

  virtual int GetSize() const = 0;

  virtual std::vector<double> GetBinCenters() const = 0;
};

/**
 * @brief The FixedBinAxisItem class represents fixed bin axis.
 *
 * Defines an axis with equidistant binning.
 */
class MVVM_MODEL_EXPORT FixedBinAxisItem : public BinnedAxisItem
{
public:
  static inline const std::string Type = "FixedBinAxis";

  explicit FixedBinAxisItem(const std::string& model_type = Type);

  std::unique_ptr<SessionItem> Clone() const override;

  void SetParameters(int nbins, double xmin, double xmax);

  static std::unique_ptr<FixedBinAxisItem> Create(int nbins, double xmin, double xmax);

  std::pair<double, double> GetRange() const override;

  int GetSize() const override;

  std::vector<double> GetBinCenters() const override;
};

/**
 * @brief The PointwiseAxisItem class represents a pointwise axis.
 *
 * Defines an axis via an array of points representing point coordinates.
 */
class MVVM_MODEL_EXPORT PointwiseAxisItem : public BinnedAxisItem
{
public:
  static inline const std::string Type = "PointwiseAxis";

  explicit PointwiseAxisItem(const std::string& model_type = Type);

  std::unique_ptr<SessionItem> Clone() const override;

  void SetParameters(const std::vector<double>& data);

  static std::unique_ptr<PointwiseAxisItem> Create(const std::vector<double>& data);

  std::pair<double, double> GetRange() const override;

  int GetSize() const override;

  std::vector<double> GetBinCenters() const override;
};

}  // namespace mvvm

#endif  // MVVM_STANDARDITEMS_AXIS_ITEMS_H_
