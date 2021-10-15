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

#include "mvvm/widgets/collapsiblebar.h"

#include "mvvm/utils/widgetutils.h"

#include <QEvent>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QMouseEvent>

namespace ModelView
{
CollapsibleBar::CollapsibleBar(QWidget* parent)
    : QFrame(parent), m_pixmapLabel(new QLabel), m_titleLabel(new QLabel)
{
  m_pixmapLabel->setPixmap(QPixmap(":/icons/chevron-down.svg"));

  auto layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 4, 0, 0);

  layout->addWidget(m_pixmapLabel, Qt::AlignLeft);
  layout->addWidget(m_titleLabel, Qt::AlignCenter);

  setFixedHeight(ModelView::Utils::HeightOfLetterM() * 1.8);
  setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
}

void CollapsibleBar::setWidget(QWidget* widget, const QString& title)
{
  m_controlledWidget = widget;
  m_titleLabel->setText(title);
  widget->installEventFilter(this);
  updatePixmap();
}

void CollapsibleBar::mousePressEvent(QMouseEvent* event)
{
  if (event->button() == Qt::LeftButton)
  {
    m_controlledWidget->setHidden(m_controlledWidget->isVisible());
  }
  updatePixmap();
}

//! Listens for widget signals and update collapse/expand icon on visibility change.

bool CollapsibleBar::eventFilter(QObject* obj, QEvent* event)
{
  bool is_event_of_interest = (event->type() == QEvent::Show || event->type() == QEvent::Hide);
  if (obj == m_controlledWidget && is_event_of_interest)
  {
    updatePixmap();
  }
  return QObject::eventFilter(obj, event);
}

//! Set pixmap depending from the visibility of the widget.

void CollapsibleBar::updatePixmap()
{
  if (m_controlledWidget->isVisible())
  {
    m_pixmapLabel->setPixmap(QPixmap(":/icons/chevron-down.svg"));
    setFrameStyle(QFrame::StyledPanel);
  }
  else
  {
    m_pixmapLabel->setPixmap(QPixmap(":/icons/chevron-right.svg"));
    setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
  }
}

}  // namespace ModelView
