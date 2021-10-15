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

#include "mvvm/widgets/fancytab.h"

#include "mvvm/widgets/widgetutils.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>

namespace
{
QColor defaultColor()
{
  static QWidget tmpWidget;
  return tmpWidget.palette().color(QPalette::Window);
}
}  // namespace

namespace ModelView
{
FancyTab::FancyTab(const QString& title, QWidget* parent)
    : QWidget(parent), m_label(new QLabel(title))
{
  ModelView::Utils::ScaleLabelFont(m_label, 1.25);
  setFixedHeight(ModelView::Utils::HeightOfLetterM() * 2.5);

  auto layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  layout->addWidget(m_label, 0, Qt::AlignCenter);
  setMouseTracking(true);
}

void FancyTab::setSelected(bool value)
{
  m_isSelected = value;
  update();
}

void FancyTab::paintEvent(QPaintEvent*)
{
  QPainter painter(this);

  if (m_widgetColor.isValid())
  {
    painter.fillRect(0, 0, size().width(), size().height(), m_widgetColor);
  }

  if (m_isSelected && isEnabled())
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

void FancyTab::enterEvent(QEvent*)
{
  if (isEnabled())
  {
    m_widgetColor = QColor(Qt::lightGray);
  }
  update();
}

void FancyTab::leaveEvent(QEvent*)
{
  if (isEnabled())
  {
    m_widgetColor = defaultColor();
  }
  update();
}

}  // namespace ModelView
