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

#include "external_property_editor.h"

#include "custom_event_filters.h"
#include "editor_helper.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/model/external_property.h>
#include <mvvm/viewmodel/custom_variants.h>

#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QToolButton>

namespace mvvm
{

ExternalPropertyEditor::ExternalPropertyEditor(QWidget* parent_widget)
    : CustomEditor(parent_widget)
    , m_text_label(new QLabel)
    , m_pixmap_label(new QLabel)
    , m_focus_filter(new LostFocusFilter(this))

{
  setMouseTracking(true);
  setAutoFillBackground(true);

  auto layout = new QHBoxLayout;
  layout->setContentsMargins(4, 0, 0, 0);

  auto button = new QToolButton;
  button->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred));
  button->setText(QLatin1String(" . . . "));
  button->setToolTip("Open selector");
  layout->addWidget(m_pixmap_label);
  layout->addWidget(m_text_label);
  layout->addStretch(1);
  layout->addWidget(button);
  setFocusPolicy(Qt::StrongFocus);
  setAttribute(Qt::WA_InputMethodEnabled);
  connect(button, &QToolButton::clicked, this, &ExternalPropertyEditor::OnButtonClicked);

  setLayout(layout);
}

void ExternalPropertyEditor::SetCallback(std::function<void(const QVariant&)> callback)
{
  m_callback = std::move(callback);
}

void ExternalPropertyEditor::OnButtonClicked()
{
  if (m_callback)
  {
    m_callback(GetData());
  }
  else
  {
    QMessageBox::warning(nullptr, "Not configured", "No external dialog configured.");
  }
}

void ExternalPropertyEditor::UpdateComponents()
{
  if (!utils::IsExternalPropertyVariant(GetData()))
  {
    throw RuntimeException("ExternalPropertyEditor: wrong variant type");
  }

  auto prop = GetData().value<ExternalProperty>();
  QPixmap pixmap(utils::DefaultPixmapSize(), utils::DefaultPixmapSize());
  pixmap.fill(QColor(QString::fromStdString(prop.GetColorName())));
  m_text_label->setText(QString::fromStdString(prop.GetText()));
  m_pixmap_label->setPixmap(pixmap);
}

}  // namespace mvvm
