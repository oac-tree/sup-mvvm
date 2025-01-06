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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
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
class MVVM_MODEL_EXPORT LineSeriesItem : public CompoundItem
{
public:
  static inline const std::string Type = "LineSeries";
  static inline const  std::string kOffset = "kOffset";

  LineSeriesItem();

  std::unique_ptr<SessionItem> Clone() const override;

  /**
   * @brief Sets the waveform data item.
   *
   * The item is stored via persistent link machinery. LineSeriesItem and LineSeriesDataItem
   * should be either from the same model or from different models sharing the same item pool.
   */
  void SetDataItem(LineSeriesDataItem* item);

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
};

}  // namespace mvvm

#endif  // MVVM_STANDARDITEMS_LINE_SERIES_ITEM_H_
