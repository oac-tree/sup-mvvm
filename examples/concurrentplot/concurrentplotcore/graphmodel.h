// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#ifndef CONCURRENTPLOTCORE_GRAPHMODEL_H
#define CONCURRENTPLOTCORE_GRAPHMODEL_H

#include "mvvm/model/sessionmodel.h"

namespace mvvm
{
class GraphViewportItem;
class ContainerItem;
}  // namespace mvvm

//! Main application model holding data for single graph. Contains Data1DItem, GraphItem and
//! ViewPortItem.

class GraphModel : public mvvm::SessionModel
{
public:
  GraphModel();

  void set_data(const std::vector<double>& data);

private:
  void init_model();
  void add_graph(mvvm::ContainerItem* container, mvvm::GraphViewportItem* viewport);
};

#endif  // CONCURRENTPLOTCORE_GRAPHMODEL_H
