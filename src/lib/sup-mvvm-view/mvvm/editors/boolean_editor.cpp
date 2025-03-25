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

#include "boolean_editor.h"

#include <mvvm/core/exceptions.h>

#include <QCheckBox>
#include <QHBoxLayout>

namespace
{
const QString true_text = "True";
const QString false_text = "False";
}  // namespace

namespace mvvm
{

BoolEditor::BoolEditor(QWidget* parent_widget)
    : CustomEditor(parent_widget), m_check_box(new QCheckBox)

{
  setAutoFillBackground(true);
  auto layout = new QHBoxLayout;
  layout->setContentsMargins(4, 0, 0, 0);
  layout->addWidget(m_check_box);
  setLayout(layout);

  connect(m_check_box, &QCheckBox::toggled, this, &BoolEditor::OnCheckBoxChange);
  setFocusProxy(m_check_box);
  m_check_box->setText(true_text);
}

void BoolEditor::OnCheckBoxChange(bool value)
{
  if (value != GetData().value<bool>())
  {
    SetDataIntern(QVariant(value));
  }
}

void BoolEditor::UpdateComponents()
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
  if (GetData().type() != QVariant::Bool)
#else
  if (GetData().typeId() != QMetaType::Bool)
#endif
  {
    throw RuntimeException("BoolEditor: wrong variant type");
  }

  bool value = GetData().value<bool>();
  m_check_box->blockSignals(true);
  m_check_box->setChecked(value);
  m_check_box->setText(value ? true_text : false_text);
  m_check_box->blockSignals(false);
}

}  // namespace mvvm
