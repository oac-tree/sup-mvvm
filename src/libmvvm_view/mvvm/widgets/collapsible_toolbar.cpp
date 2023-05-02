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

#include "mvvm/widgets/collapsible_toolbar.h"

#include <mvvm/widgets/style_utils.h>
#include <mvvm/widgets/widget_utils.h>

#include <QLabel>
#include <QToolBar>
#include <QToolButton>
#include <QVBoxLayout>

namespace mvvm
{
CollapsibleToolBar::CollapsibleToolBar(QWidget *parent)
    : QFrame(parent)
    , m_tool_bar(new QToolBar)
    , m_expand_button(new QToolButton)
    , m_label(new QLabel)
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_tool_bar);

  m_tool_bar->setIconSize(utils::NarrowToolBarIconSize());

  // make it narrow, buttons closer to each other
  m_tool_bar->setStyleSheet(
      "QToolBar { padding-bottom: 0px; margin-bottom: 0px; } QToolButton { padding: 0px; margin: "
      "0px; }");

  m_expand_button->setIcon(utils::GetIcon("chevron-down.svg"));
  auto on_expand_click = [this]()
  {
    m_expanded = !m_expanded;
    UpdateToolBar();
  };
  connect(m_expand_button, &QToolButton::clicked, this, on_expand_click);
  m_tool_bar->addWidget(m_expand_button);

  m_tool_bar->addWidget(m_label);
  InsertStrech();

  setFixedHeight(utils::NarrowToolBarIconSize().height() * 1.5);

  UpdateToolBar();
}

//! Set text next to collapse/expand icon.

void CollapsibleToolBar::SetText(const QString &text, const QString &text_tooltip)
{
  mvvm::utils::ScaleLabelFont(m_label, 0.9);
  m_label->setText(text);
  m_label->setToolTip(text_tooltip);
}

//! Sets the widget which will be collapsed/expanded.

void CollapsibleToolBar::SetControlledWidget(QWidget *widget)
{
  m_controlled_widget = widget;
}

void CollapsibleToolBar::AddWidget(QWidget *widget)
{
  // Feature of QToolBar: returns action on adding any widget or button.
  // These actions can be used to hide widgets from the toolbar (see ::UpdateToolBar)
  auto action = m_tool_bar->addWidget(widget);
  m_toolbar_actions.append(action);  // to hide/show elements later
}

void CollapsibleToolBar::AddWidgets(const QList<QWidget *> &widgets)
{
  for (auto widget : widgets)
  {
    AddWidget(widget);
  }
}

void CollapsibleToolBar::AddAction(QAction *action)
{
  m_tool_bar->addAction(action);
  m_toolbar_actions.append(action);
}

void CollapsibleToolBar::AddActions(const QList<QAction *> actions)
{
  for (auto action : actions)
  {
    AddAction(action);
  }
}

QToolBar *CollapsibleToolBar::GetToolBar()
{
  return m_tool_bar;
}

//! Updates toolbar appearance depending on collapsed/expaned status

void CollapsibleToolBar::UpdateToolBar()
{
  UpdateIcon();

  // show/hide controlled widget
  if (m_controlled_widget)
  {
    m_controlled_widget->setVisible(m_expanded);
  }

  // show/hide toolbar elements
  for (auto action : m_toolbar_actions)
  {
    action->setVisible(m_expanded);
  }
}

//! Updates collapse/expand icon.

void CollapsibleToolBar::UpdateIcon()
{
  if (m_expanded)
  {
    m_expand_button->setIcon(utils::GetIcon("chevron-down.svg"));
    setFrameStyle(QFrame::StyledPanel);
  }
  else
  {
    m_expand_button->setIcon(utils::GetIcon("chevron-right.svg"));
    setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
  }
}

void CollapsibleToolBar::InsertStrech()
{
  auto empty = new QWidget(this);
  empty->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  m_tool_bar->addWidget(empty);
}

}  // namespace mvvm
