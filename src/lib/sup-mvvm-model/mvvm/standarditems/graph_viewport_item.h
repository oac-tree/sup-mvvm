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

#ifndef MVVM_STANDARDITEMS_GRAPH_VIEWPORT_ITEM_H_
#define MVVM_STANDARDITEMS_GRAPH_VIEWPORT_ITEM_H_

#include <mvvm/standarditems/viewport_item.h>

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

  using ViewportItem::ViewportItem;
  std::unique_ptr<SessionItem> Clone(bool make_unique_id = true) const override;

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

#endif  // MVVM_STANDARDITEMS_GRAPH_VIEWPORT_ITEM_H_
