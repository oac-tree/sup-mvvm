// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#ifndef PLOTGRAPHSCORE_GRAPHMODEL_H
#define PLOTGRAPHSCORE_GRAPHMODEL_H

#include "mvvm/model/applicationmodel.h"

namespace mvvm
{
class GraphViewportItem;
class ContainerItem;
}  // namespace mvvm

namespace plotgraphs
{

//! Main application model to demonstrate how to plot data.
//! Contains a few Data1DItem's, GraphItem's, and ViewPortItem. Provides functionality to easily
//! add and remove graphs.

class GraphModel : public mvvm::ApplicationModel
{
public:
  GraphModel();

  void AddGraph();

  void RemoveGraph();

  void RandomizeGraphs();

private:
  mvvm::GraphViewportItem* GetViewport();
  mvvm::ContainerItem* GetDataContainer();
};

}  // namespace plotgraphs

#endif  // PLOTGRAPHSCORE_GRAPHMODEL_H
