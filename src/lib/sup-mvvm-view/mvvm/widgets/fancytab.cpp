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

#include "fancytab.h"

#include "widget_utils.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>

namespace
{

QColor GetDefaultColor()
{
  static QWidget tmpWidget;
  return tmpWidget.palette().color(QPalette::Window);
}

}  // namespace

namespace mvvm
{
FancyTab::FancyTab(const QString& title, QWidget* parent_widget)
    : QWidget(parent_widget), m_label(new QLabel(title))
{
  mvvm::utils::ScaleLabelFont(m_label, 1.25);
  setFixedHeight(mvvm::utils::HeightOfLetterM() * 2.5);

  auto layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  layout->addWidget(m_label, 0, Qt::AlignCenter);
  setMouseTracking(true);
}

void FancyTab::SetSelected(bool value)
{
  m_is_selected = value;
  update();
}

void FancyTab::paintEvent(QPaintEvent*)
{
  QPainter painter(this);

  if (m_widget_color.isValid())
  {
    painter.fillRect(0, 0, size().width(), size().height(), m_widget_color);
  }

  if (m_is_selected && isEnabled())
  {
    painter.fillRect(
        QRectF(QPointF(0, size().height() - 2), QPointF(size().width(), size().height())),
        QColor("#0d4283"));
  }
}

void FancyTab::mousePressEvent(QMouseEvent* event)
{
  if (isEnabled() && event->button() == Qt::LeftButton)
  {
    emit clicked();
  }
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
void FancyTab::enterEvent(QEvent*)
#else
void FancyTab::enterEvent(QEnterEvent*)
#endif
{
  if (isEnabled())
  {
    m_widget_color = QColor(Qt::lightGray);
  }
  update();
}

void FancyTab::leaveEvent(QEvent*)
{
  if (isEnabled())
  {
    m_widget_color = GetDefaultColor();
  }
  update();
}

}  // namespace mvvm
