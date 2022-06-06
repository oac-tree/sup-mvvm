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

#include "mvvm/plotting/graph_info_formatter.h"

#include "mvvm/utils/string_utils.h"

#include <qcustomplot.h>

#include <sstream>

namespace
{

QCPGraph* find_graph_nearby(QCustomPlot* custom_plot, double x, double y)
{
  double widget_px = custom_plot->xAxis->coordToPixel(x);
  double widget_py = custom_plot->yAxis->coordToPixel(y);
  return dynamic_cast<QCPGraph*>(custom_plot->plottableAt(QPointF(widget_px, widget_py)));
}

int getBin(const QCPGraph* graph, double x)
{
  const int key_start = graph->findBegin(x);
  const int key_end = graph->findBegin(x, false);  // false = do not expand range
  if (key_end == key_start || key_end == graph->dataCount())
    return key_start;
  return (x - graph->dataSortKey(key_start)) <= (graph->dataSortKey(key_end) - x) ? key_start
                                                                                  : key_end;
}

struct Context
{
  double xpos{0.0};
  double ypos{0.0};
  bool close_to_graph{false};
  int nx{0};
  double value{0.0};
};

std::string compose_string(const Context& context)
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

}  // namespace

namespace mvvm
{

std::string GraphInfoFormatter::status_string(QCustomPlot* custom_plot, double x, double y) const
{
  Context context{x, y};

  if (auto qcp_graph = find_graph_nearby(custom_plot, x, y); qcp_graph)
  {
    context.close_to_graph = true;
    context.nx = getBin(qcp_graph, x);
    context.value = qcp_graph->dataMainValue(context.nx);
  }

  return compose_string(context);
}

}  // namespace mvvm
