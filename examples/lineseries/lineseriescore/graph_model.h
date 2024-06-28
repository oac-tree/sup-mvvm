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

#ifndef PLOTGRAPHS_PLOTGRAPHSCORE_GRAPH_MODEL_H_
#define PLOTGRAPHS_PLOTGRAPHSCORE_GRAPH_MODEL_H_

#include <mvvm/model/application_model.h>

namespace mvvm
{
class ChartViewportItem;
class ContainerItem;
}  // namespace mvvm

namespace lineseries
{

/**
 * @brief The GraphModel class holds all plot-related items and provides API to add/remove
 * lineseries.
 */
class GraphModel : public mvvm::ApplicationModel
{
public:
  GraphModel();

  /**
   * @brief Adds line series to the model.
   */
  void AddGraph();

  /**
   * @brief Removes last line series, and its data.
   */
  void RemoveGraph();

  /**
   * @brief Puts random noise to graphs.
   */
  void RandomizeGraphs();

  /**
   * @brief Returns viewport containing line series items.
   */
  mvvm::ChartViewportItem* GetViewport();

  /**
   * @brief Returns container with data items.
   */
  mvvm::ContainerItem* GetDataContainer();
};

}  // namespace lineseries

#endif  // PLOTGRAPHS_PLOTGRAPHSCORE_GRAPH_MODEL_H_
