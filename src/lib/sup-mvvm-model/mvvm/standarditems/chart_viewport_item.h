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

#ifndef MVVM_STANDARDITEMS_CHART_VIEWPORTITEM_ITEM_H_
#define MVVM_STANDARDITEMS_CHART_VIEWPORTITEM_ITEM_H_

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
  static inline const std::string Type = "ChartViewport";

  ChartViewportItem();

  std::vector<LineSeriesItem*> GetLineSeries() const;

  int GetLineSeriesCount() const;

protected:
  std::pair<double, double> GetDataXRange() const override;
  std::pair<double, double> GetDataYRange() const override;
};


}  // namespace mvvm

#endif  // MVVM_STANDARDITEMS_CHART_VIEWPORTITEM_ITEM_H_
