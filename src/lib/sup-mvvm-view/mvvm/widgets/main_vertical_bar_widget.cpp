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

#include "main_vertical_bar_widget.h"

#include "fancytab.h"

#include <QButtonGroup>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QStatusBar>
#include <QToolButton>
#include <QVBoxLayout>

namespace
{

QString GetBackgroundColorName(const QColor& base_color)
{
  return base_color.name(QColor::HexRgb);
}

QString GetPressedColorName(const QColor& base_color)
{
  return QString("#dae7ed");
}

QString GetHoverColorName(const QColor& base_color)
{
  return base_color.lighter(150).name(QColor::HexRgb);
}

QString GetCheckedColorName(const QColor& base_color)
{
  return base_color.lighter(120).name(QColor::HexRgb);
}

QString GetButtonStyleString(const QColor& base_color)
{
  static const QString result =
      "QToolButton { border: none; color: white; background-color: %1;}"
      "QToolButton:pressed { color: black; background-color: %2; }"
      "QToolButton:hover { color: white; background-color: %3; }"
      "QToolButton:checked { color: white; background-color: %4; }";
  return result.arg(GetBackgroundColorName(base_color), GetPressedColorName(base_color),
                    GetHoverColorName(base_color), GetCheckedColorName(base_color));
}

}  // namespace

namespace mvvm
{
MainVerticalBarWidget::MainVerticalBarWidget(QWidget* parent)
    : QFrame(parent)
    , m_stacked_widget(new QStackedWidget)
    , m_button_layout(new QVBoxLayout)
    , m_button_group(new QButtonGroup(this))
    , m_status_bar(new QStatusBar)
    , m_base_color("#005291")
{
  m_button_layout->setContentsMargins(0, 0, 0, 0);
  m_button_layout->setSpacing(0);

  auto layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  auto widgets_layout = new QVBoxLayout;
  widgets_layout->setContentsMargins(0, 0, 0, 0);
  widgets_layout->addWidget(m_stacked_widget);
  widgets_layout->addWidget(m_status_bar);
  m_status_bar->hide();

  layout->addLayout(m_button_layout);
  layout->addLayout(widgets_layout);

  setFrameStyle(QFrame::Sunken);

  connect(m_button_group, &QButtonGroup::idClicked, this, &MainVerticalBarWidget::SetCurrentIndex);
}

MainVerticalBarWidget::~MainVerticalBarWidget() = default;

void MainVerticalBarWidget::AddWidget(QWidget* widget, const QString& title, const QIcon& icon)
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

void MainVerticalBarWidget::SetCurrentIndex(int index)
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

void MainVerticalBarWidget::AddSpacer()
{
  m_filler_button = CreateViewSelectionButton();
  m_filler_button->setMinimumSize(5, 5);
  m_filler_button->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
  m_filler_button->setEnabled(false);
  m_button_layout->insertWidget(m_button_layout->count(), m_filler_button);

  UpdateViewSelectionButtonsGeometry();
}

void MainVerticalBarWidget::SetBaseColor(const QColor& color)
{
  m_base_color = color;
}

QStatusBar* MainVerticalBarWidget::GetStatusBar()
{
  return m_status_bar;
}

QToolButton* MainVerticalBarWidget::CreateViewSelectionButton()
{
  auto result = new QToolButton;
  result->setCheckable(true);
  result->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
  //  result->setToolButtonStyle(Qt::ToolButtonIconOnly);
  result->setStyleSheet(GetButtonStyleString(m_base_color));
  return result;
}

void MainVerticalBarWidget::UpdateViewSelectionButtonsGeometry()
{
  if (m_button_group->buttons().isEmpty())
  {
    return;
  }

  const QFontMetrics font_metrics = m_button_group->buttons().first()->fontMetrics();

  // Find the maximum text extents
  int max_text_width = 0;
  int max_text_height = 0;
  for (auto b : m_button_group->buttons())
  {
    const auto r = font_metrics.boundingRect(b->text());
    max_text_width = std::max(max_text_width, r.width());
    max_text_height = std::max(max_text_height, r.height());
  }

  // calculate the button extent by width (width == height!). Ensure an extent of 70 for normal
  // DPI devices (legacy value)
  const int margin = font_metrics.boundingRect("M").width();
  const int button_extent = std::max(50, max_text_width + 2 * margin);

  // calculate the icon extent by height (width == height!)
  const int icon_extent = 0.9 * button_extent - margin - max_text_height;

  // set new values in all buttons
  for (auto button : m_button_group->buttons())
  {
    button->setFixedSize(button_extent, button_extent);
    button->setIconSize({icon_extent, icon_extent});
  }
  // set fixed width in filler and progress bar
  if (m_filler_button)
  {
    m_filler_button->setFixedWidth(button_extent);
  }
}

}  // namespace mvvm
