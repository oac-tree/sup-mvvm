/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_PLOTTING_GRAPH_CANVAS_H_
#define MVVM_PLOTTING_GRAPH_CANVAS_H_

#include <mvvm/view_export.h>

#include <QWidget>
#include <memory>

namespace mvvm
{

class GraphViewportItem;
class SceneAdapterInterface;

//! Widget to show scientific figure with multiple 1D graphs.
//! Contains embedded QCustomPlot widget, shows content of GraphViewportItem.

class MVVM_VIEW_EXPORT GraphCanvas : public QWidget
{
  Q_OBJECT

public:
  explicit GraphCanvas(QWidget* parent = nullptr);
  ~GraphCanvas() override;

  void SetItem(GraphViewportItem* viewport_item);

  void SetViewportToContent(double left, double top, double right, double bottom);

  void SetViewportToContent();

  void SetAxisMargins(int left, int top, int right, int bottom);

signals:
  void axisMarginsChanged(int left, int top, int right, int bottom);

private:
  struct GraphCanvasImpl;
  std::unique_ptr<GraphCanvasImpl> p_impl;
};

}  // namespace mvvm

#endif  // MVVM_PLOTTING_GRAPH_CANVAS_H_
