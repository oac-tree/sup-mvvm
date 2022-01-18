// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#ifndef MVVM_STANDARDITEMS_AXISITEMS_H
#define MVVM_STANDARDITEMS_AXISITEMS_H

//! @file axisitems.h
//! Collection of axis items for 1D and 2D data/plotting support.

#include "mvvm/model/compounditem.h"

#include <memory>
#include <vector>

namespace mvvm
{

class TextItem;

//! Base class for all axes items. Has min, max defined, but nothing else.

class MVVM_MODEL_EXPORT BasicAxisItem : public CompoundItem
{
public:
  explicit BasicAxisItem(const std::string& model_type);

  double GetMin() const;
  void SetMin(double value);

  double GetMax() const;
  void SetMax(double value);

protected:
  void RegisterMinMax();
};

//! Item to represent viewport axis.
//! Serves as a counterpart of QCPAxis from QCustomPlot. Intended to cary title, fonts etc.

class MVVM_MODEL_EXPORT ViewportAxisItem : public BasicAxisItem
{
public:
  static inline const std::string Type = "ViewportAxis";

  explicit ViewportAxisItem(const std::string& model_type = Type);

  TextItem* GetTitle() const;

  std::pair<double, double> GetRange() const;
  void SetRange(double lower, double upper);

  bool IsInLog() const;
  void SetInLog(bool value);
};

//! Item to represent an axis with arbitrary binning.
//! Base class to define an axis with specific binning (fixed, variable). Used in Data1DItem and
//! Data2Ditem to store 1d and 2d data.  Doesn't carry any appearance info (e.g. axis title, label
//! size, etc) and thus not intended for direct plotting.

class MVVM_MODEL_EXPORT BinnedAxisItem : public BasicAxisItem
{
public:
  explicit BinnedAxisItem(const std::string& model_type);

  virtual std::pair<double, double> range() const = 0;

  virtual int size() const = 0;

  virtual std::vector<double> binCenters() const = 0;
};

//! Item to represent fixed bin axis.
//! Defines an axis with equidistant binning.

class MVVM_MODEL_EXPORT FixedBinAxisItem : public BinnedAxisItem
{
public:
  static inline const std::string Type = "FixedBinAxis";

  static inline const std::string P_NBINS = "P_NBINS";
  explicit FixedBinAxisItem(const std::string& model_type = Type);

  void setParameters(int nbins, double xmin, double xmax);

  static std::unique_ptr<FixedBinAxisItem> create(int nbins, double xmin, double xmax);

  std::pair<double, double> range() const override;

  int size() const override;

  std::vector<double> binCenters() const override;
};

//! Item to represent pointwise axis.
//! Defines an axis via array of points representing point coordinates.

class MVVM_MODEL_EXPORT PointwiseAxisItem : public BinnedAxisItem
{
public:
  static inline const std::string Type = "PointwiseAxis";
  explicit PointwiseAxisItem(const std::string& model_type = Type);

  void setParameters(const std::vector<double>& data);

  static std::unique_ptr<PointwiseAxisItem> create(const std::vector<double>& data);

  std::pair<double, double> range() const override;

  int size() const override;

  std::vector<double> binCenters() const override;
};

}  // namespace mvvm

#endif  // MVVM_STANDARDITEMS_AXISITEMS_H
