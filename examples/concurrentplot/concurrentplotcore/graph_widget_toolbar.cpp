/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "graph_widget_toolbar.h"

#include <QDebug>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QVBoxLayout>

namespace
{
const int kInitialValue = 50;
const int kMinValue = 0;
const int kMaxValue = 100;
}  // namespace

namespace concurrentplot
{

GraphWidgetToolBar::GraphWidgetToolBar(QWidget* parent)
    : QToolBar(parent)
    , m_value_box(new QSpinBox)
    , m_value_slider(new QSlider)
    , m_delay_spinbox(new QSpinBox)
    , m_progressbar(new QProgressBar)
    , m_cancel_button(new QPushButton)
{
  const int toolbar_icon_size = 24;
  setIconSize(QSize(toolbar_icon_size, toolbar_icon_size));

  InitValueElements();
  AddWideSeparator();

  InitDelayElements();
  AddWideSeparator();

  InitFlowElements();
}

//! Set progress bar to given value.

void GraphWidgetToolBar::OnProgressChanged(int value)
{
  m_progressbar->setValue(value);
}

void GraphWidgetToolBar::AddWideSeparator()
{
  addWidget(new QLabel(" "));
  addSeparator();
  addWidget(new QLabel(" "));
}

//! Inits elements to edit initial simulation parameter.

void GraphWidgetToolBar::InitValueElements()
{
  const QString tooltip =
      "Input parameter of background simulation.\n"
      "Any change triggers new simulation and subsequent replot.";

  auto label = new QLabel("Amplitude");
  label->setToolTip(tooltip);
  addWidget(label);

  // Value box setup.
  m_value_box->setValue(kInitialValue);
  m_value_box->setRange(kMinValue, kMaxValue);
  m_value_box->setMinimumWidth(100);
  m_value_box->setToolTip(tooltip);
  m_value_box->setKeyboardTracking(false);
  addWidget(m_value_box);

  auto on_value_changed = [this](int value)
  {
    m_value_slider->setValue(value);
    this->valueChanged(value);
  };
  connect(m_value_box, QOverload<int>::of(&QSpinBox::valueChanged), on_value_changed);

  // Slider setup.
  m_value_slider->setOrientation(Qt::Horizontal);
  m_value_slider->setRange(kMinValue, kMaxValue);
  m_value_slider->setValue(kInitialValue);
  m_value_slider->setMaximumWidth(400);
  m_value_slider->setToolTip(tooltip);
  addWidget(m_value_slider);

  auto on_slider_changed = [this](int value) { m_value_box->setValue(value); };
  connect(m_value_slider, &QSlider::valueChanged, on_slider_changed);
}

//! Inits label and sping box to edit simulation slowdown factor.

void GraphWidgetToolBar::InitDelayElements()
{
  const QString tooltip =
      "Delay affects the duration of background simulation.\n"
      "Given in microseconds per one point of the graph.";
  auto label = new QLabel("Delay");
  label->setToolTip(tooltip);
  addWidget(label);

  m_delay_spinbox->setRange(0, 50000);
  m_delay_spinbox->setMinimumWidth(100);
  addWidget(m_delay_spinbox);

  connect(m_delay_spinbox, QOverload<int>::of(&QSpinBox::valueChanged), this,
          &GraphWidgetToolBar::delayChanged);
  m_delay_spinbox->setValue(1000);
}

//! Inits progress bar and button to cancel running simulation.

void GraphWidgetToolBar::InitFlowElements()
{
  m_progressbar->setMaximumWidth(300);
  addWidget(m_progressbar);

  auto placeholder = new QWidget;
  placeholder->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
  addWidget(placeholder);

  m_cancel_button->setText("Cancel Simulation");
  addWidget(m_cancel_button);
  connect(m_cancel_button, &QPushButton::pressed, this, &GraphWidgetToolBar::cancelPressed);
}

}  // namespace concurrentplot
