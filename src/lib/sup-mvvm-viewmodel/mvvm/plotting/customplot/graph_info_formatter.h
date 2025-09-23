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

#ifndef MVVM_PLOTTING_CUSTOMPLOT_GRAPH_INFO_FORMATTER_H_
#define MVVM_PLOTTING_CUSTOMPLOT_GRAPH_INFO_FORMATTER_H_

#include <mvvm/plotting/customplot/status_string_formatter_interface.h>

class QCustomPlot;
class QCPGraph;

namespace mvvm
{

struct CanvasGraphContext
{
  double xpos{0.0};
  double ypos{0.0};
  bool close_to_graph{false};
  int nx{0};
  double value{0.0};
};

/**
 * @brief Get string representation of the given plot context.
 */
std::string GetContextString(const CanvasGraphContext& context);

/**
 * @brief Returns a graph located not far from the given coordinates.
 */
QCPGraph* FindGraphNearby(QCustomPlot* custom_plot, double xpos, double ypos);

/**
 * @brief The GraphInfoFormatter class generates status string for current mouse position.
 *
 * It is used to get information when mouse is hovered over a graph in QCustomPlot.
 */
class GraphInfoFormatter : public StatusStringFormatterInterface
{
public:
  /**
   * @brief Returns status string representing graph nearby.
   *
   * @param custom_plot The QCustomPlot main canvas.
   * @param xpos Mouse x-coordinate
   * @param ypos Mouse y-coordinate
   * @return String descirbing current mouse position
   */
  std::string status_string(QCustomPlot* custom_plot, double xpos, double ypos) const override;
};

}  // namespace mvvm

#endif  // MVVM_PLOTTING_CUSTOMPLOT_GRAPH_INFO_FORMATTER_H_
