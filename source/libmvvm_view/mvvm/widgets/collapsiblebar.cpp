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

#include "mvvm/widgets/widgetutils.h"
#include "mvvm/resources/resources.h"

#include <QEvent>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QMouseEvent>

namespace mvvm
{
CollapsibleBar::CollapsibleBar(QWidget* parent)
    : QFrame(parent), m_pixmap_label(new QLabel), m_title_label(new QLabel)
{
  InitIconResources();
  m_pixmap_label->setPixmap(QPixmap(":/icons/chevron-down.svg"));

  auto layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 4, 0, 0);

  layout->addWidget(m_pixmap_label, Qt::AlignLeft);
  layout->addWidget(m_title_label, Qt::AlignCenter);

  setFixedHeight(mvvm::utils::HeightOfLetterM() * 1.8);
  setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
}

void CollapsibleBar::setWidget(QWidget* widget, const QString& title)
{
  m_controlled_widget = widget;
  m_title_label->setText(title);
  widget->installEventFilter(this);
  updatePixmap();
}

void CollapsibleBar::mousePressEvent(QMouseEvent* event)
{
  if (event->button() == Qt::LeftButton)
  {
    m_controlled_widget->setHidden(m_controlled_widget->isVisible());
  }
  updatePixmap();
}

//! Listens for widget signals and update collapse/expand icon on visibility change.

bool CollapsibleBar::eventFilter(QObject* obj, QEvent* event)
{
  bool is_event_of_interest = (event->type() == QEvent::Show || event->type() == QEvent::Hide);
  if (obj == m_controlled_widget && is_event_of_interest)
  {
    updatePixmap();
  }
  return QObject::eventFilter(obj, event);
}

//! Set pixmap depending from the visibility of the widget.

void CollapsibleBar::updatePixmap()
{
  if (m_controlled_widget->isVisible())
  {
    m_pixmap_label->setPixmap(QPixmap(":/icons/chevron-down.svg"));
    setFrameStyle(QFrame::StyledPanel);
  }
  else
  {
    m_pixmap_label->setPixmap(QPixmap(":/icons/chevron-right.svg"));
    setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
  }
}

}  // namespace ModelView
