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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "main_vertical_bar_widget.h"

#include "widget_utils.h"

#include <mvvm/core/exceptions.h>

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
  return base_color.darker(150).name(QColor::HexRgb);
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

/**
 * @brief Returns maximum width and height calculated over all button's text labels.
 */
std::pair<int, int> GetMaxWidthHeight(const QList<QAbstractButton*>& buttons)
{
  int max_text_width{0};
  int max_text_height{0};

  if (buttons.isEmpty())
  {
    return {max_text_width, max_text_height};
  }

  const QFontMetrics font_metrics = buttons.first()->fontMetrics();

  // Find the maximum text extents
  for (auto button : buttons)
  {
    const auto rect = font_metrics.boundingRect(button->text());
    max_text_width = std::max(max_text_width, rect.width());
    max_text_height = std::max(max_text_height, rect.height());
  }
  return {max_text_width, max_text_height};
}

/**
 * @brief A margin between toolbar buttons.
 */
int GetMargin()
{
  return mvvm::utils::WidthOfLetterM() * 0.75;
}

int GetMinimumToolBarWidth()
{
  return mvvm::utils::WidthOfLetterM() * 7;
}

}  // namespace

namespace mvvm
{
MainVerticalBarWidget::MainVerticalBarWidget(QWidget* parent_widget)
    : QFrame(parent_widget)
    , m_stacked_widget(new QStackedWidget)
    , m_button_layout(new QVBoxLayout)
    , m_button_group(new QButtonGroup(this))
    , m_status_bar(new QStatusBar)
    , m_base_color(Qt::darkGray)
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
  const int index = m_stacked_widget->addWidget(widget);

  auto button = CreateControlButton();
  button->setText(title);
  button->setIcon(icon);
  button->setToolTip(title);
  m_button_group->addButton(button.get(), index);

  // passing ownership
  m_button_layout->insertWidget(m_button_layout->count(), button.release());

  UpdateButtonLayout();
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
    throw RuntimeException("MainVerticalBarWidget: can't find button for index");
  }
}

void MainVerticalBarWidget::AddSpacer()
{
  auto button = CreateControlButton();
  button->setMinimumSize(5, 5);
  button->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
  button->setEnabled(false);
  m_filler_button = button.get();

  // passing ownership
  m_button_layout->insertWidget(m_button_layout->count(), button.release());

  UpdateButtonLayout();
}

void MainVerticalBarWidget::SetBaseColor(const QColor& color)
{
  m_base_color = color;
}

QStatusBar* MainVerticalBarWidget::GetStatusBar()
{
  return m_status_bar;
}

std::unique_ptr<QToolButton> MainVerticalBarWidget::CreateControlButton()
{
  auto result = std::make_unique<QToolButton>();
  result->setCheckable(true);
  result->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
  result->setStyleSheet(GetButtonStyleString(m_base_color));
  return result;
}

void MainVerticalBarWidget::UpdateButtonLayout()
{
  if (m_button_group->buttons().isEmpty())
  {
    return;
  }

  auto [max_text_width, max_text_height] = GetMaxWidthHeight(m_button_group->buttons());

  const auto margin = GetMargin();
  const int button_extent = std::max(GetMinimumToolBarWidth(), max_text_width + 2 * margin);
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
