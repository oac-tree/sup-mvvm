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

#include "mvvm/editors/booleditor.h"

#include <QCheckBox>
#include <QHBoxLayout>
#include <stdexcept>

namespace
{
const QString true_text = "True";
const QString false_text = "False";
}  // namespace

namespace mvvm
{

BoolEditor::BoolEditor(QWidget* parent) : CustomEditor(parent), m_checkBox(new QCheckBox)

{
  setAutoFillBackground(true);
  auto layout = new QHBoxLayout;
  layout->setContentsMargins(4, 0, 0, 0);
  layout->addWidget(m_checkBox);
  setLayout(layout);

  connect(m_checkBox, &QCheckBox::toggled, this, &BoolEditor::OnCheckBoxChange);
  setFocusProxy(m_checkBox);
  m_checkBox->setText(true_text);
}

bool BoolEditor::IsPersistent() const
{
  return true;
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
  if (GetData().type() != QVariant::Bool)
  {
    throw std::runtime_error("BoolEditor::update_components() -> Error. Wrong variant type");
  }

  bool value = GetData().value<bool>();
  m_checkBox->blockSignals(true);
  m_checkBox->setChecked(value);
  m_checkBox->setText(value ? true_text : false_text);
  m_checkBox->blockSignals(false);
}

}  // namespace mvvm
