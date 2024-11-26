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

#ifndef MVVM_STANDARDITEMS_VIEWPORT_ITEM_H_
#define MVVM_STANDARDITEMS_VIEWPORT_ITEM_H_

#include <mvvm/model/compound_item.h>

namespace mvvm
{

class ViewportAxisItem;

/**
 * @brief The ViewportItem class is a base class for all viewports representing 1D, and 2D data.
 *
 * Contains x,y axis, to display graphs, line series, or 2D colormaps.
 */
class MVVM_MODEL_EXPORT ViewportItem : public CompoundItem
{
public:
  static inline const std::string kXaxis = "kXaxis";
  static inline const std::string kYaxis = "kYaxis";
  static inline const std::string kItems = "kItems";

  explicit ViewportItem(const std::string& item_type);

  /**
   * @brief Returns x-axis.
   */
  ViewportAxisItem* GetXAxis() const;

  /**
   * @brief Returns y-axis.
   */
  ViewportAxisItem* GetYAxis() const;

  /**
   * @brief Sets the range of x,y window to show all data.
   */
  virtual void SetViewportToContent();

  /**
   * @brief Sets range of x,y window to show all data.
   *
   * Allows adding an additional margin to automatically calculated axis range. Margins are given in
   * relative units wrt calculated axis range. For example, setViewportToContent(0.0, 0.1, 0.0, 0.1)
   * will set axes to show all graphs with 10% gap above and below graph's max and min.
   */
  virtual void SetViewportToContent(double left, double top, double right, double bottom);

protected:
  /**
   * @brief Register tags to attach axes.
   */
  void RegisterAxes();

  /**
   * @brief Returns the area occupied by the data along the x-axis.
   */
  virtual std::pair<double, double> GetDataXRange() const = 0;

  /**
   * @brief Returns the area occupied by the data along the y-axis.
   */
  virtual std::pair<double, double> GetDataYRange() const = 0;
};

}  // namespace mvvm

#endif  // MVVM_STANDARDITEMS_VIEWPORT_ITEM_H_
