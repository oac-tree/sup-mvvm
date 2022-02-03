/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

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
