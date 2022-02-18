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

#include "mvvm/editors/externalpropertyeditor.h"

#include "mvvm/editors/customeventfilters.h"
#include "mvvm/editors/styleutils.h"
#include "mvvm/model/externalproperty.h"
#include "mvvm/viewmodel/customvariants.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QToolButton>
#include <stdexcept>

namespace mvvm
{

ExternalPropertyEditor::ExternalPropertyEditor(QWidget* parent)
    : CustomEditor(parent)
    , m_textLabel(new QLabel)
    , m_pixmapLabel(new QLabel)
    , m_focusFilter(new LostFocusFilter(this))

{
  setMouseTracking(true);
  setAutoFillBackground(true);

  auto layout = new QHBoxLayout;
  layout->setContentsMargins(4, 0, 0, 0);

  auto button = new QToolButton;
  button->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred));
  button->setText(QLatin1String(" . . . "));
  button->setToolTip("Open selector");
  layout->addWidget(m_pixmapLabel);
  layout->addWidget(m_textLabel);
  layout->addStretch(1);
  layout->addWidget(button);
  setFocusPolicy(Qt::StrongFocus);
  setAttribute(Qt::WA_InputMethodEnabled);
  connect(button, &QToolButton::clicked, this, &ExternalPropertyEditor::buttonClicked);

  setLayout(layout);
}

void ExternalPropertyEditor::setCallback(std::function<void(const QVariant&)> callback)
{
  m_callback = std::move(callback);
}

void ExternalPropertyEditor::buttonClicked()
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
    throw std::runtime_error("Error. Wrong variant type (ExternalProperty is required).");

  auto prop = GetData().value<ExternalProperty>();
  QPixmap pixmap(style::DefaultPixmapSize(), style::DefaultPixmapSize());
  pixmap.fill(QColor(QString::fromStdString(prop.GetColorName())));
  m_textLabel->setText(QString::fromStdString(prop.GetText()));
  m_pixmapLabel->setPixmap(pixmap);
}

}  // namespace mvvm
