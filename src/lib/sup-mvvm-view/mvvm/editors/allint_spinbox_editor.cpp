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

#include "allint_spinbox_editor.h"

#include "allint_spinbox.h"

#include <mvvm/utils/i_limited_integer.h>
#include <mvvm/utils/limited_integer_helper.h>
#include <mvvm/viewmodel/variant_converter.h>

#include <QVBoxLayout>

namespace mvvm
{

AllIntSpinBoxEditor::AllIntSpinBoxEditor(QWidget* parent_widget)
    : CustomEditor(parent_widget), m_allint_editor(new AllIntSpinBox)
{
  setAutoFillBackground(true);
  setFocusPolicy(Qt::StrongFocus);
  m_allint_editor->setFocusPolicy(Qt::StrongFocus);
  m_allint_editor->setKeyboardTracking(false);

  auto layout = new QVBoxLayout;
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  layout->addWidget(m_allint_editor);

  connect(m_allint_editor, &AllIntSpinBox::valueChanged, this,
          &AllIntSpinBoxEditor::OnValueChanged);

  setLayout(layout);

  setFocusProxy(m_allint_editor);
}

bool AllIntSpinBoxEditor::IsPersistent() const
{
  return false;
}

void AllIntSpinBoxEditor::SetRange(const variant_t& lower_limit, const variant_t& upper_limit)
{
  m_user_lower_limit = lower_limit;
  m_user_upper_limit = upper_limit;
}

variant_t AllIntSpinBoxEditor::GetLowerLimit() const
{
  if (auto limited_integer = m_allint_editor->GetLimitedInteger(); limited_integer)
  {
    return limited_integer->GetLowerBoundAsVariant();
  }
  return {};
}

variant_t AllIntSpinBoxEditor::GetUpperLimit() const
{
  if (auto limited_integer = m_allint_editor->GetLimitedInteger(); limited_integer)
  {
    return limited_integer->GetUpperBoundAsVariant();
  }
  return {};
}

void AllIntSpinBoxEditor::OnValueChanged(const QVariant& value)
{
  SetDataIntern(value);
}

void AllIntSpinBoxEditor::UpdateComponents()
{
  auto limited_int =
      CreateLimitedInteger(GetStdVariant(GetData()), m_user_lower_limit, m_user_upper_limit);
  m_allint_editor->SetLimitedInteger(std::move(limited_int));
}

}  // namespace mvvm
