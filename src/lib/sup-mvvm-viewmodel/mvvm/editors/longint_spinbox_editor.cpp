/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Rafael Silva (EXT)
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

#include "longint_spinbox_editor.h"

#include <mvvm/editors/longint_spinbox.h>
#include <mvvm/viewmodel/custom_variants.h>

#include <QVBoxLayout>
#include <stdexcept>

namespace mvvm
{

LongIntSpinBoxEditor::LongIntSpinBoxEditor(QWidget* parent)
    : CustomEditor(parent), m_longint_editor(new LongIntSpinBox)
{
  setAutoFillBackground(true);
  setFocusPolicy(Qt::StrongFocus);
  m_longint_editor->setFocusPolicy(Qt::StrongFocus);
  m_longint_editor->setKeyboardTracking(false);

  auto layout = new QVBoxLayout;
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  layout->addWidget(m_longint_editor);

  connect(m_longint_editor, &LongIntSpinBox::valueChanged, this,
          &LongIntSpinBoxEditor::OnValueChanged);

  setLayout(layout);

  setFocusProxy(m_longint_editor);
}

void LongIntSpinBoxEditor::SetRange(qint64 minimum, qint64 maximum)
{
  m_longint_editor->setMinimum(minimum);
  m_longint_editor->setMaximum(maximum);
}

void LongIntSpinBoxEditor::SetSingleStep(qint64 step)
{
  m_longint_editor->setSingleStep(step);
}

bool LongIntSpinBoxEditor::IsPersistent() const
{
  return true;
}

int64 LongIntSpinBoxEditor::GetMinimum() const
{
  return m_longint_editor->minimum();
}

int64 LongIntSpinBoxEditor::GetMaximum() const
{
  return m_longint_editor->maximum();
}

void LongIntSpinBoxEditor::OnValueChanged(const qint64 value)
{
  SetDataIntern(QVariant::fromValue(value));
}

void LongIntSpinBoxEditor::UpdateComponents()
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
  const auto type_id = GetData().type();
  if (type_id != QVariant::LongLong
      && utils::GetQtVariantName(GetData()) != constants::kInt64QtTypeName)
#else
  if (GetData().typeId() != QMetaType::LongLong)
#endif
  {
    throw std::runtime_error(
        "LongIntSpinBoxEditor::UpdateComponents() -> Error. Wrong variant type");
  }

  m_longint_editor->setValue(GetData().value<qint64>());
}

}  // namespace mvvm
