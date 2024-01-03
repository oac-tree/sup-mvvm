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

#ifndef CONCURRENTPLOT_CONCURRENTPLOTCORE_GRAPH_MODEL_H_
#define CONCURRENTPLOT_CONCURRENTPLOTCORE_GRAPH_MODEL_H_

#include "mvvm/model/application_model.h"

namespace mvvm
{
class GraphViewportItem;
class ContainerItem;
}  // namespace mvvm

namespace concurrentplot
{

//! Main application model holding data for single graph. Contains Data1DItem, GraphItem and
//! ViewPortItem.

class GraphModel : public mvvm::ApplicationModel
{
public:
  GraphModel();

  void SetData(const std::vector<double>& data);

private:
  void PopulateModel();
  void AddGraph(mvvm::ContainerItem* container, mvvm::GraphViewportItem* viewport);
};

}  // namespace concurrentplot

#endif  // CONCURRENTPLOT_CONCURRENTPLOTCORE_GRAPH_MODEL_H_
