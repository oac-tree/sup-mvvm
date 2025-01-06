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

#include "allint_spinbox.h"

#include <mvvm/standarditems/editor_constants.h>
#include <mvvm/utils/limited_integer.h>
#include <mvvm/viewmodel/variant_converter.h>

#include <QEvent>
#include <QLineEdit>

namespace
{
bool has_digits_only(const std::string &str)
{
  return str.find_first_not_of("0123456789+-") == std::string::npos;
}
}  // namespace

namespace mvvm
{

AllIntSpinBox::AllIntSpinBox(QWidget *parent) : QAbstractSpinBox(parent)
{
  connect(this, &QAbstractSpinBox::editingFinished, this, &AllIntSpinBox::OnEditingFinished);
}

void AllIntSpinBox::SetInteger(std::unique_ptr<ILimitedInteger> value)
{
  m_value = std::move(value);
  UpdateTextField();
}

AllIntSpinBox::~AllIntSpinBox() = default;

QVariant AllIntSpinBox::value() const
{
  if (!m_value)
  {
    return {};
  }

  return GetQtVariant(m_value->GetValueAsVariant());
}

QValidator::State AllIntSpinBox::validate(QString &input, int &pos) const
{
  if (input.isEmpty())
  {
    // An empty field is a valid intermediate state
    return QValidator::Intermediate;
  }

  if (input.length() == 1 && (input[0] == '+' || input[0] == '-'))
  {
    // A single sign character is a valid intermediate state
    return QValidator::Intermediate;
  }

  if (!has_digits_only(input.toStdString()))
  {
    return QValidator::Invalid;
  }

  // We are performing final conversion check on board of ILimitedInteger::SetValueFromText.
  // Let's not worry here if the value is outside limits.

  return QValidator::Acceptable;
}

void AllIntSpinBox::stepBy(int steps)
{
  if (!m_value)
  {
    return;
  }

  if (m_value->StepBy(steps))
  {
    UpdateTextField();
    m_cached_value_was_changed = true;
  }
}

void AllIntSpinBox::CheckNotify()
{
  if (!m_value)
  {
    return;
  }

  if (m_cached_value_was_changed)
  {
    m_cached_value_was_changed = false;
    emit valueChanged(GetQtVariant(m_value->GetValueAsVariant()));
  }
}

QAbstractSpinBox::StepEnabled AllIntSpinBox::stepEnabled() const
{
  if (isReadOnly() || !m_value)
  {
    return StepNone;
  }

  QAbstractSpinBox::StepEnabled step_enabled{StepNone};

  if (!m_value->IsAtMaximum())
  {
    step_enabled |= StepUpEnabled;
  }

  if (!m_value->IsAtMinimum())
  {
    step_enabled |= StepDownEnabled;
  }
  return step_enabled;
}

void AllIntSpinBox::setValue(const QVariant &value)
{
  if (!m_value)
  {
    return;
  }

  if (m_value->SetValueFromVariant(GetStdVariant(value)))
  {
    m_cached_value_was_changed = true;
    UpdateTextField();
  }

  CheckNotify();
}

void AllIntSpinBox::OnEditingFinished()
{
  auto text_value = text();
  int pos = 0;  // unused
  if (validate(text_value, pos) == QValidator::Acceptable)
  {
    if (m_value->SetValueFromText(text_value.toStdString()))
    {
      m_cached_value_was_changed = true;
    }
  }

  CheckNotify();
}

void AllIntSpinBox::UpdateTextField()
{
  const auto value_text = QString::fromStdString(m_value->GetValueAsText());
  if (value_text != text())
  {
    lineEdit()->setText(value_text);
    lineEdit()->selectAll();
  }
}

}  // namespace mvvm
