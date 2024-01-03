/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "status_label.h"

#include "widget_utils.h"

#include <QColor>
#include <QFont>
#include <QPainter>

namespace
{
const int kDefaultInfoBarTextSize = 8;
const int kDefaultInfoBarHeight = 24;
}  // namespace

namespace mvvm
{

StatusLabel::StatusLabel(QWidget* parent)
    : QFrame(parent), m_font("Monospace", kDefaultInfoBarTextSize, QFont::Normal, false)
{
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  setFixedHeight(kDefaultInfoBarHeight);
}

void StatusLabel::setText(const QString& text)
{
  m_text = text;
  update();
}

void StatusLabel::setFont(const QFont& font)
{
  m_font = font;
  update();
}

void StatusLabel::setPointSize(int pointSize)
{
  m_font.setPointSize(pointSize);
  update();
}

void StatusLabel::setAlignment(Qt::Alignment alignment)
{
  m_alignment = alignment;
  update();
}

void StatusLabel::paintEvent(QPaintEvent* event)
{
  QFrame::paintEvent(event);

  QPainter painter(this);
  painter.setBrush(QColor(Qt::black));
  painter.setPen(QColor(Qt::black));
  painter.setFont(m_font);

  QRect bbox(0, 0, geometry().width(), geometry().height());
  const int gap(utils::WidthOfLetterM() / 2);  // make it smaller
  auto textRect = bbox.adjusted(gap, 0, gap, 0);

  painter.fillRect(bbox, QColor(Qt::white));
  painter.drawText(textRect, static_cast<int>(m_alignment), m_text);
}

}  // namespace mvvm
