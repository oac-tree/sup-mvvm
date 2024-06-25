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

  LineSeriesItem();

  using CompoundItem::CompoundItem;

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  void SetDataItem(LineSeriesDataItem* item);

  LineSeriesDataItem* GetDataItem();
  const LineSeriesDataItem* GetDataItem() const;

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

  std::string GetNamedColor() const;

  void SetNamedColor(const std::string& named_color);

  PenItem* GetPenItem() const;
};

}  // namespace mvvm

#endif  // MVVM_STANDARDITEMS_LINE_SERIES_ITEM_H_
