/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
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

#include "mvvm/model/compound_item.h"

namespace mvvm
{

class ViewportAxisItem;

//! Base class to represent 2D viewport.
//! Contains x,y axis, indended to display graphs or 2d colormaps.

class MVVM_MODEL_EXPORT ViewportItem : public CompoundItem
{
public:
  static inline const std::string kXaxis = "kXaxis";
  static inline const std::string kYaxis = "kYaxis";
  static inline const std::string kItems = "kItems";

  explicit ViewportItem(const std::string& item_type);

  ViewportAxisItem* GetXAxis() const;

  ViewportAxisItem* GetYAxis() const;

  virtual void SetViewportToContent(double left, double top, double right, double bottom);

  virtual void SetViewportToContent();

protected:
  void RegisterAxes();

  virtual std::pair<double, double> GetDataXRange() const = 0;
  virtual std::pair<double, double> GetDataYRange() const = 0;
};

}  // namespace mvvm

#endif  // MVVM_STANDARDITEMS_VIEWPORTITEM_H
