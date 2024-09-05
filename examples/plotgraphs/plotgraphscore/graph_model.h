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
class GraphViewportItem;
class ContainerItem;
}  // namespace mvvm

namespace plotgraphs
{

/**
 * @brief  Main application model to demonstrate how to plot data.
 *
 * Contains a few Data1DItem's, GraphItem's, and ViewPortItem. Provides functionality to easily add
 * and remove graphs.
 */
class GraphModel : public mvvm::ApplicationModel
{
public:
  GraphModel();

  /**
   * @brief Adds graphto the model.
   *
   * Internally it adds Data1DItem carrying data points, and GraphItem containing presentation
   * details and linked to Data1DItem.
   */
  void AddGraph();

  /**
   * @brief Removes last added graph.
   */
  void RemoveGraph();

  /**
   * @brief Put random noise to graphs.
   */
  void RandomizeGraphs();

  /**
   * @brief Undo last action.
   */
  void Undo();

  /**
   * @brief Redo previously undone action.
   */
  void Redo();

  /**
   * @brief  Returns viewport item containig graph items.
   */
  mvvm::GraphViewportItem* GetViewport();

  /**
   * @brief Returns container with data items.
   */
  mvvm::ContainerItem* GetDataContainer();
};

}  // namespace plotgraphs

#endif  // PLOTGRAPHS_PLOTGRAPHSCORE_GRAPH_MODEL_H_
