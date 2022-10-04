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

#ifndef CONCURRENTPLOT_CONCURRENTPLOTCORE_GRAPHMODEL_H
#define CONCURRENTPLOT_CONCURRENTPLOTCORE_GRAPHMODEL_H

#include "mvvm/model/application_model.h"

namespace mvvm
{
class GraphViewportItem;
class ContainerItem;
}  // namespace mvvm

//! Main application model holding data for single graph. Contains Data1DItem, GraphItem and
//! ViewPortItem.

class GraphModel : public mvvm::ApplicationModel
{
public:
  GraphModel();

  void set_data(const std::vector<double>& data);

private:
  void init_model();
  void add_graph(mvvm::ContainerItem* container, mvvm::GraphViewportItem* viewport);
};

#endif  // CONCURRENTPLOT_CONCURRENTPLOTCORE_GRAPHMODEL_H
