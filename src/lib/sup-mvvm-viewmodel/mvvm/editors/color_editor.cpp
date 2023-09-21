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

#include "color_editor.h"

#include <mvvm/editors/custom_event_filters.h>
#include <mvvm/editors/style_utils.h>

#include <QColorDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <stdexcept>

namespace mvvm
{

ColorEditor::ColorEditor(QWidget* parent)
    : CustomEditor(parent)
    , m_text_label(new QLabel)
    , m_pixmap_label(new QLabel)
    , m_focus_filter(new LostFocusFilter(this))

{
  setMouseTracking(true);
  setAutoFillBackground(true);

  auto layout = new QHBoxLayout;
  layout->setContentsMargins(4, 0, 0, 0);

  layout->addWidget(m_pixmap_label);
  layout->addWidget(m_text_label);  // no color name, only color rectangle
  layout->addStretch(1);
  setFocusPolicy(Qt::StrongFocus);
  setAttribute(Qt::WA_InputMethodEnabled);

  setLayout(layout);
}

void ColorEditor::mousePressEvent(QMouseEvent*)
{
  // temporarily installing filter to prevent loss of focus caused by too insistent dialog
  installEventFilter(m_focus_filter);

  auto new_color = QColorDialog::getColor(GetCurrentColor());

  removeEventFilter(m_focus_filter);

  if (new_color.isValid())
  {
    // converting color to variant with QString containing color name
    SetDataIntern(QVariant::fromValue(new_color.name()));
    UpdateComponents();
  }
}

QColor ColorEditor::GetCurrentColor() const
{
  return QColor(GetData().toString());
}

void ColorEditor::UpdateComponents()
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
  if (GetData().type() != QVariant::String)
#else
  if (GetData().typeId() != QMetaType::QString)
#endif
  {
    throw std::runtime_error("ColorEditor::update_components() -> Error. Wrong variant type");
  }

  QPixmap pixmap(style::DefaultPixmapSize(), style::DefaultPixmapSize());
  pixmap.fill(GetCurrentColor());
  m_text_label->setText(GetCurrentColor().name());
  m_pixmap_label->setPixmap(pixmap);
}

}  // namespace mvvm
