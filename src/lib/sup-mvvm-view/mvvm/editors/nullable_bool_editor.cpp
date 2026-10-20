/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "nullable_bool_editor.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/providers/custom_variants.h>

#include <QComboBox>
#include <QVBoxLayout>

namespace
{
const int kNoValueIndex = 0;
const int kTrueIndex = 1;
const int kFalseIndex = 2;
}  // namespace

namespace mvvm
{

NullableBoolEditor::NullableBoolEditor(QWidget* parent_widget)
    : CustomEditor(parent_widget), m_combo_box(new QComboBox)
{
  setAutoFillBackground(true);

  auto layout = new QVBoxLayout;
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_combo_box);
  setLayout(layout);

  m_combo_box->addItem("default", QVariant());
  m_combo_box->addItem("True", QVariant(true));
  m_combo_box->addItem("False", QVariant(false));

  connect(m_combo_box, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
          &NullableBoolEditor::OnIndexChanged);
  setFocusProxy(m_combo_box);
}

void NullableBoolEditor::SetPlaceholderText(const QString& text)
{
  m_combo_box->setItemText(kNoValueIndex, text);
}

QString NullableBoolEditor::GetPlaceholderText() const
{
  return m_combo_box->itemText(kNoValueIndex);
}

QComboBox* NullableBoolEditor::GetComboBox()
{
  return m_combo_box;
}

void NullableBoolEditor::OnIndexChanged(int index)
{
  SetDataIntern(m_combo_box->itemData(index));
}

void NullableBoolEditor::UpdateComponents()
{
  const auto data = GetData();
  if (data.isValid() && !utils::IsBoolVariant(data))
  {
    throw RuntimeException("NullableBoolEditor: wrong variant type");
  }

  int index = kNoValueIndex;
  if (data.isValid())
  {
    index = data.toBool() ? kTrueIndex : kFalseIndex;
  }

  m_combo_box->blockSignals(true);
  m_combo_box->setCurrentIndex(index);
  m_combo_box->blockSignals(false);
}

}  // namespace mvvm
