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

#ifndef CONCURRENTPLOT_CONCURRENTPLOTCORE_GRAPHWIDGETTOOLBAR_H
#define CONCURRENTPLOT_CONCURRENTPLOTCORE_GRAPHWIDGETTOOLBAR_H

#include <QToolBar>

class QSlider;
class QSpinBox;
class QPushButton;
class QProgressBar;

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
  void onProgressChanged(int value);

private:
  void add_wide_separator();
  void init_value_elements();
  void init_delay_elements();
  void init_flow_elements();

  QSpinBox* value_box{nullptr};         //! Input parameter for simulation.
  QSlider* value_slider{nullptr};       //! Input parameter for simulation.
  QSpinBox* delay_spinbox{nullptr};     //! Factor defining how slow simulation is.
  QProgressBar* progressbar{nullptr};   //! Simulation progressbar.
  QPushButton* cancel_button{nullptr};  //! Cancels simulation.
};

#endif  // CONCURRENTPLOT_CONCURRENTPLOTCORE_GRAPHWIDGETTOOLBAR_H
