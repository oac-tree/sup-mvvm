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

#include "graph_info_formatter.h"

#include <mvvm/utils/string_utils.h>

#include <qcustomplot.h>

#include <sstream>

namespace mvvm
{

namespace
{

std::int32_t getBin(const QCPGraph* graph, double xpos)
{
  const std::int32_t key_start = graph->findBegin(xpos);
  const std::int32_t key_end = graph->findBegin(xpos, false);  // false = do not expand range
  if (key_end == key_start || key_end == graph->dataCount())
  {
    return key_start;
  }
  return (xpos - graph->dataSortKey(key_start)) <= (graph->dataSortKey(key_end) - xpos) ? key_start
                                                                                        : key_end;
}

}  // namespace

QCPGraph* FindGraphNearby(QCustomPlot* custom_plot, double xpos, double ypos)
{
  const double widget_px = custom_plot->xAxis->coordToPixel(xpos);
  const double widget_py = custom_plot->yAxis->coordToPixel(ypos);
  return dynamic_cast<QCPGraph*>(custom_plot->plottableAt(QPointF(widget_px, widget_py)));
}

std::string GetContextString(const CanvasGraphContext& context)
{
  std::ostringstream ostr;
  ostr << "[x: " << mvvm::utils::DoubleToString(context.xpos, 3) << ", ";
  ostr << "y: " << mvvm::utils::DoubleToString(context.ypos, 3) << "] ";
  if (context.close_to_graph)
  {
    ostr << "[binx: " << context.nx << "] ";
    ostr << "[value: " << mvvm::utils::ScientificDoubleToString(context.value) << "]";
  }
  return ostr.str();
}

std::string GraphInfoFormatter::status_string(QCustomPlot* custom_plot, double xpos,
                                              double ypos) const
{
  CanvasGraphContext context{xpos, ypos};

  if (auto qcp_graph = FindGraphNearby(custom_plot, xpos, ypos); qcp_graph)
  {
    context.close_to_graph = true;
    context.nx = getBin(qcp_graph, xpos);
    context.value = qcp_graph->dataMainValue(context.nx);
  }

  return GetContextString(context);
}

}  // namespace mvvm
