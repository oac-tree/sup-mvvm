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

#ifndef MVVM_STANDARDITEMS_LINE_SERIES_ITEM_H_
#define MVVM_STANDARDITEMS_LINE_SERIES_ITEM_H_

#include <mvvm/model/compound_item.h>

namespace mvvm
{

class PenItem;
class LineSeriesDataItem;

/**
 * @brief The LineSeriesItem class represents 1D graph.
 *
 * It is a counterprart of QtChart::QLineSeries in mvvm world. It contains plot properties and a
 * link to LineSeriesDataItem. LineSeriesItem is intended for plotting via ChartViewportItem.
 */
class LineSeriesItem : public CompoundItem
{
public:
  static constexpr std::string_view kOffset = "kOffset";
  static constexpr std::string_view kPointsVisible = "kPointsVisible";
  static constexpr std::string_view kMarkerSize = "kMarkerSize";
  static constexpr double kDefaultMarkerSize = 3.0;

  LineSeriesItem();

  std::unique_ptr<SessionItem> Clone() const override;

  /**
   * @brief Sets the waveform data item.
   *
   * The item is stored via persistent link machinery. LineSeriesItem and LineSeriesDataItem
   * should be either from the same model or from different models sharing the same item pool.
   */
  void SetDataItem(const LineSeriesDataItem* item);

  /**
   * @brief Returns waveform data to which this LineSeriesItem is linked (const version).
   */
  const LineSeriesDataItem* GetDataItem() const;

  /**
   * @brief Returns waveform data to which this LineSeriesItem is linked (non-const version).
   */
  LineSeriesDataItem* GetDataItem();

  /**
   * @brief Returns the value of x-axis offset.
   *
   * X-coordinates of all points during the plot will be shifted by this number. It doesn't
   * affect values itself.
   */
  double GetXOffset() const;

  /**
   * @brief Set the value of x-axis offset.
   *
   * X-coordinates of all points during the plot will be shifted by this number. It doesn't
   * affect values itself.
   */
  void SetXOffset(double value);

  /**
   * @brief Returns named color (html notation).
   */
  std::string GetNamedColor() const;

  /**
   * @brief Sets the named color.
   */
  void SetNamedColor(const std::string& named_color);

  /**
   * @brief Returns underlying pen item.
   */
  PenItem* GetPenItem() const;

  /**
   * @brief Returns x-values.
   *
   * Values are obtained from underlying LineSeriesDataItem, x-offset is applied.
   */
  std::vector<double> GetBinCenters() const;

  /**
   * @brief Returns y-values.
   */
  std::vector<double> GetValues() const;

  /**
   * @brief Returns the value of kDisplayed property.
   */
  bool IsDisplayed() const;

  /**
   * @brief Makes line series shown or hidden on a chart view.
   */
  void SetDisplayed(bool value);

  /**
   * @brief Checks whether points are visible.
   */
  bool IsPointsVisible() const;

  /**
   *  @brief Sets whether points are visible.
   */
  void SetPointsVisible(bool value);

  /**
   * @brief Returns the size of markers for points.
   */
  double GetMarkerSize() const;

  /**
   * @brief Set the size of markers for points.
   */
  void SetMarkerSize(double value);
};

template <>
struct item_traits<LineSeriesItem>
{
  static constexpr std::string_view type_name() noexcept { return "LineSeries"; }
};

}  // namespace mvvm

#endif  // MVVM_STANDARDITEMS_LINE_SERIES_ITEM_H_
