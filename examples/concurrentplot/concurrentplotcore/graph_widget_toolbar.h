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

#ifndef CONCURRENTPLOT_CONCURRENTPLOTCORE_GRAPH_WIDGET_TOOLBAR_H_
#define CONCURRENTPLOT_CONCURRENTPLOTCORE_GRAPH_WIDGET_TOOLBAR_H_

#include <QToolBar>

class QSlider;
class QSpinBox;
class QPushButton;
class QProgressBar;

namespace concurrentplot
{

//! Toolbar for GraphWidget. Contains elements to steer background simulation.

class GraphWidgetToolBar : public QToolBar
{
  Q_OBJECT

public:
  explicit GraphWidgetToolBar(QWidget* parent = nullptr);

signals:
  void valueChanged(int value);
  void delayChanged(int delay);
  void cancelPressed();

public slots:
  void OnProgressChanged(int value);

private:
  void AddWideSeparator();
  void InitValueElements();
  void InitDelayElements();
  void InitFlowElements();

  QSpinBox* m_value_box{nullptr};         //! Input parameter for simulation.
  QSlider* m_value_slider{nullptr};       //! Input parameter for simulation.
  QSpinBox* m_delay_spinbox{nullptr};     //! Factor defining how slow simulation is.
  QProgressBar* m_progressbar{nullptr};   //! Simulation progressbar.
  QPushButton* m_cancel_button{nullptr};  //! Cancels simulation.
};

}  // namespace concurrentplot

#endif  // CONCURRENTPLOT_CONCURRENTPLOTCORE_GRAPH_WIDGET_TOOLBAR_H_
