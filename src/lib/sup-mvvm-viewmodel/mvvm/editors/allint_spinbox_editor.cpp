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

#include "allint_spinbox_editor.h"

#include <mvvm/editors/allint_spinbox.h>
#include <mvvm/utils/limited_integer_helper.h>
#include <mvvm/viewmodel/variant_converter.h>

#include <QVBoxLayout>
#include <QDebug>

namespace mvvm
{

AllIntSpinBoxEditor::AllIntSpinBoxEditor(QWidget* parent)
    : CustomEditor(parent), m_allint_editor(new AllIntSpinBox)
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
  return true;
}

void AllIntSpinBoxEditor::SetRange(const variant_t& lower_limit, const variant_t& upper_limit)
{
  m_lower_limit = lower_limit;
  m_upper_limit = upper_limit;
}

void AllIntSpinBoxEditor::OnValueChanged(const QVariant& value)
{
  qDebug() << "AllIntSpinBoxEditor::OnValueChanged()() 1.1";
  SetDataIntern(value);
}

void AllIntSpinBoxEditor::UpdateComponents()
{
  qDebug() << "AllIntSpinBoxEditor::UpdateComponents()() 1.1";

  auto limited_int = CreateLimitedInteger(GetStdVariant(GetData()), m_lower_limit, m_upper_limit);
  m_allint_editor->SetInteger(std::move(limited_int));
}

}  // namespace mvvm
