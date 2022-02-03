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

#include "mvvm/model/sessionmodel.h"

namespace mvvm
{
class GraphViewportItem;
class ContainerItem;
}  // namespace mvvm

namespace PlotGraphs
{

//! Main application model to demonstrate how to plot data.
//! Contains few Data1DItem's, GraphItem's and ViewPortItem. Provides functionality to easily
//! add and remove graphs.

class GraphModel : public mvvm::SessionModel
{
public:
  GraphModel();

  void addGraph();

  void removeGraph();

  void randomizeGraphs();

private:
  mvvm::GraphViewportItem* viewport();
  mvvm::ContainerItem* dataContainer();

  void populateModel();
};

}  // namespace PlotGraphs

#endif  // PLOTGRAPHSCORE_GRAPHMODEL_H
