// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#ifndef MVVM_STANDARDITEMS_GRAPHVIEWPORTITEM_H
#define MVVM_STANDARDITEMS_GRAPHVIEWPORTITEM_H

#include "mvvm/standarditems/viewportitem.h"

namespace mvvm
{

class GraphItem;

//! 2D viewport specialized for showing multiple GraphItem's.
//! Provides calculation of viewport's x-axis and y-axis range basing on GraphItem data.
//! Provides functionality to hide selected graphs.

class MVVM_MODEL_EXPORT GraphViewportItem : public ViewportItem
{
public:
  static inline const std::string Type = "GraphViewport";

  explicit GraphViewportItem(const std::string& model_type = Type);

  int GetGraphCount() const;

  std::vector<GraphItem*> GetGraphItems() const;

  std::vector<GraphItem*> GetVisibleGraphItems() const;

  void SetVisible(const std::vector<GraphItem*>& visible_graph_items);

  void SetAllVisible();

protected:
  std::pair<double, double> GetDataXRange() const override;
  std::pair<double, double> GetDataYRange() const override;
};

}  // namespace mvvm

#endif  // MVVM_STANDARDITEMS_GRAPHVIEWPORTITEM_H
