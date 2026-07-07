/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#ifndef MVVM_STANDARDITEMS_VIEWPORT_ITEM_H_
#define MVVM_STANDARDITEMS_VIEWPORT_ITEM_H_

#include <mvvm/model/compound_item.h>
#include <mvvm/standarditems/viewport_types.h>

namespace mvvm
{

class ViewportAxisItem;

/**
 * @brief The ViewportItem class is a base class for all viewports representing 1D, and 2D data.
 *
 * Contains x,y axis, to display graphs, line series, or 2D colormaps.
 */
class ViewportItem : public CompoundItem
{
public:
  static constexpr std::string_view kXaxis = "kXaxis";
  static constexpr std::string_view kYaxis = "kYaxis";

  explicit ViewportItem(const std::string& item_type);

  ViewportItem(const ViewportItem&) = delete;
  ViewportItem(ViewportItem&&) = delete;
  ViewportItem& operator=(const ViewportItem&) = delete;
  ViewportItem& operator=(ViewportItem&&) = delete;
  ~ViewportItem() override = default;

  /**
   * @brief Returns x-axis.
   */
  ViewportAxisItem* GetXAxis() const;

  /**
   * @brief Returns y-axis.
   */
  ViewportAxisItem* GetYAxis() const;

  /**
   * @brief Returns the area occupied by the data along the x-axis.
   */
  std::pair<double, double> GetDataXRange() const;

  /**
   * @brief Returns the area occupied by the data along the y-axis.
   */
  std::pair<double, double> GetDataYRange() const;

  /**
   * @brief Returns viewport area.
   */
  ViewportArea GetViewportArea() const;

  /**
   * @brief Returns area occupied by the data.
   */
  ViewportArea GetDataArea() const;

  /**
   * @brief Sets viewport area.
   */
  void SetViewportArea(const ViewportArea& area) const;

  /**
   * @brief Sets the range of x,y window to show all data.
   */
  void SetViewportToContent() const;

  /**
   * @brief Sets range of x,y window to show all data.
   *
   * Allows adding an additional margin to automatically calculated axis range. Margins are given in
   * relative units wrt calculated axis range.  For example,
   * setViewportToContent({0.0, 0.1, 0.0, 0.1}) // left, top, right, bottom
   * will set axes to show all graphs with 10% gap above and below graph's max and min.
   */
  void SetViewportToContent(const ViewportMargins& margins) const;

protected:
  /**
   * @brief Registers X and Y axes.
   */
  void RegisterAxes();

  /**
   * @brief Returns the area occupied by the data along the x-axis.
   */
  virtual std::pair<double, double> GetDataXRangeImpl() const = 0;

  /**
   * @brief Returns the area occupied by the data along the y-axis.
   */
  virtual std::pair<double, double> GetDataYRangeImpl() const = 0;
};

}  // namespace mvvm

#endif  // MVVM_STANDARDITEMS_VIEWPORT_ITEM_H_
