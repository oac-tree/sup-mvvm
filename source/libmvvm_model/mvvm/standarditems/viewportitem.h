/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_STANDARDITEMS_VIEWPORTITEM_H
#define MVVM_STANDARDITEMS_VIEWPORTITEM_H

#include "mvvm/model/compounditem.h"

namespace mvvm
{

class ViewportAxisItem;

//! Base class to represent 2D viewport.
//! Contains x,y axis, indended to display graphs or 2d colormaps.

class MVVM_MODEL_EXPORT ViewportItem : public CompoundItem
{
public:
  static inline const std::string P_XAXIS = "P_XAXIS";
  static inline const std::string P_YAXIS = "P_YAXIS";
  static inline const std::string T_ITEMS = "T_ITEMS";

  explicit ViewportItem(const std::string& item_type);

  ViewportAxisItem* xAxis() const;

  ViewportAxisItem* yAxis() const;

  virtual void setViewportToContent(double left, double top, double right, double bottom);

  virtual void setViewportToContent();

protected:
  void register_xy_axes();

  virtual std::pair<double, double> data_xaxis_range() const = 0;
  virtual std::pair<double, double> data_yaxis_range() const = 0;
};

}  // namespace mvvm

#endif  // MVVM_STANDARDITEMS_VIEWPORTITEM_H
