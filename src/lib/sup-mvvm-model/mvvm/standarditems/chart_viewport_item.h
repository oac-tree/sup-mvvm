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

#ifndef MVVM_STANDARDITEMS_CHART_VIEWPORT_ITEM_H_
#define MVVM_STANDARDITEMS_CHART_VIEWPORT_ITEM_H_

#include <mvvm/standarditems/viewport_item.h>

namespace mvvm
{

class LineSeriesItem;

/**
 * @brief The ChartViewportItem class is intended to show multiple LineSeriesItem.
 *
 * It is a counterpart of QtChart::QChart in mvvm world.
 */
class ChartViewportItem : public mvvm::ViewportItem
{
public:
  static constexpr std::string_view kAnimation = "kAnimation";
  static constexpr std::string_view kGridAnimation = "Grid axis animation";
  static constexpr std::string_view kSeriesAnimation = "Line series animation";
  static constexpr std::string_view kLegendEnabled = "kLegendEnabled";

  ChartViewportItem();

  std::unique_ptr<SessionItem> Clone() const override;

  std::vector<LineSeriesItem*> GetLineSeries() const;

  std::size_t GetLineSeriesCount() const;

protected:
  std::pair<double, double> GetDataXRange() const override;
  std::pair<double, double> GetDataYRange() const override;
};

template <>
struct item_traits<ChartViewportItem>
{
  static constexpr std::string_view type_name() noexcept { return "ChartViewport"; }
};

}  // namespace mvvm

#endif  // MVVM_STANDARDITEMS_CHART_VIEWPORT_ITEM_H_
