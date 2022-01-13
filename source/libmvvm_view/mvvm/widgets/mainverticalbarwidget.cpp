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

#include "mvvm/widgets/mainverticalbarwidget.h"

#include "mvvm/widgets/fancytab.h"

#include <QButtonGroup>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QToolButton>
#include <QVBoxLayout>

namespace mvvm
{
MainVerticalBarWidget::MainVerticalBarWidget(QWidget* parent)
    : QFrame(parent)
    , m_stacked_widget(new QStackedWidget)
    , m_button_layout(new QVBoxLayout)
    , m_button_group(new QButtonGroup)
{
  m_button_layout->setMargin(0);
  m_button_layout->setSpacing(0);

  auto layout = new QHBoxLayout(this);
  layout->addLayout(m_button_layout);
  layout->addWidget(m_stacked_widget);
  layout->setMargin(0);
  layout->setSpacing(0);
  layout->setContentsMargins(0, 0, 0, 0);

  setFrameStyle(QFrame::Sunken);

  connect(m_button_group, &QButtonGroup::idClicked, this, &MainVerticalBarWidget::setCurrentIndex);
}

MainVerticalBarWidget::~MainVerticalBarWidget() = default;

void MainVerticalBarWidget::addWidget(QWidget* widget, const QString& title, const QIcon& icon)
{
  int index = m_stacked_widget->addWidget(widget);

  auto button = CreateViewSelectionButton();
  m_button_layout->insertWidget(m_button_layout->count(), button);

  button->setText(title);
  button->setIcon(icon);
  button->setToolTip(title);
  m_button_group->addButton(button, index);

  UpdateViewSelectionButtonsGeometry();
}

void MainVerticalBarWidget::setCurrentIndex(int index)
{
  if (auto button = m_button_group->button(index); button)
  {
    button->setChecked(true);
    m_stacked_widget->setCurrentIndex(index);
  }
  else
  {
    throw std::runtime_error("Can't find button for index");
  }
}

void MainVerticalBarWidget::addSpacer()
{
  m_filler_button = CreateViewSelectionButton();
  m_filler_button->setMinimumSize(5, 5);
  m_filler_button->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
  m_filler_button->setEnabled(false);
  m_button_layout->insertWidget(m_button_layout->count(), m_filler_button);

  UpdateViewSelectionButtonsGeometry();
}

QToolButton* MainVerticalBarWidget::CreateViewSelectionButton()
{
  //    static const QString viewSelectionButtonStyle =
  //        "QToolButton { border: none; color: white; background-color: qlineargradient(x1: 0, "
  //        "y1: 0, x2: 1, y2: 0, stop : 0 #153b4c, stop : 1 #347a9c);}        "
  //        "QToolButton:pressed { "
  //        " color: black; background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 0, stop: 0 "
  //        "#97a8b0, stop: "
  //        "1 #dae7ed); }"
  //        "QToolButton:hover { "
  //        " color: white; background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 0, stop: 0 "
  //        "#254b5c, stop: 1 #448aac); }"
  //        "QToolButton:checked { "
  //        " color: black; background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 0, stop: 0 "
  //        "#97a8b0, stop: "
  //        "1 #dae7ed);"
  //        "} ";

  static const QString viewSelectionButtonStyle =
      "QToolButton { border: none; color: white; background-color: #005291;}        "
      "QToolButton:pressed { "
      " color: black; background-color: #dae7ed; }"
      "QToolButton:hover { "
      " color: white; background-color: #0071c8; }"
      "QToolButton:checked { "
      " color: black; background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 0, stop: 0 "
      "#97a8b0, stop: "
      "1 #dae7ed);"
      "} ";

  auto result = new QToolButton;
  result->setCheckable(true);
  result->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
  //  result->setToolButtonStyle(Qt::ToolButtonIconOnly);
  result->setStyleSheet(viewSelectionButtonStyle);
  return result;
}

void MainVerticalBarWidget::UpdateViewSelectionButtonsGeometry()
{
  if (m_button_group->buttons().isEmpty())
  {
    return;
  }

  const QFontMetrics fontMetrics = m_button_group->buttons().first()->fontMetrics();

  // Find the maximum text extents
  int maxTextWidth = 0;
  int maxTextHeight = 0;
  for (auto b : m_button_group->buttons())
  {
    const auto r = fontMetrics.boundingRect(b->text());
    maxTextWidth = std::max(maxTextWidth, r.width());
    maxTextHeight = std::max(maxTextHeight, r.height());
  }

  // calculate the button extent by width (width == height!). Ensure an extent of 70 for normal
  // DPI devices (legacy value)
  const int margin = fontMetrics.boundingRect("M").width();
  const int buttonExtent = std::max(50, maxTextWidth + 2 * margin);

  // calculate the icon extent by height (width == height!)
  const int iconExtent = 0.9 * buttonExtent - margin - maxTextHeight;

  // set new values in all buttons
  for (auto b : m_button_group->buttons())
  {
    b->setFixedSize(buttonExtent, buttonExtent);
    b->setIconSize({iconExtent, iconExtent});
  }
  // set fixed width in filler and progress bar
  if (m_filler_button)
  {
    m_filler_button->setFixedWidth(buttonExtent);
  }
}

}  // namespace ModelView
