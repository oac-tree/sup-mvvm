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

#include "allint_sphinbox.h"

#include <mvvm/standarditems/editor_constants.h>
#include <mvvm/utils/limited_integer.h>
#include <mvvm/viewmodel/variant_converter.h>

#include <QEvent>
#include <QLineEdit>

namespace mvvm
{

AllIntSpinBox::AllIntSpinBox(QWidget *parent) : QAbstractSpinBox(parent)
{
  // Propagate editor value to internal value
  connect(this, &QAbstractSpinBox::editingFinished, this, &AllIntSpinBox::OnEditingFinished);
  // Propagate value changes to editor and handle step enabled states
  connect(this, &AllIntSpinBox::valueChanged, this, &AllIntSpinBox::updateEdit);
}

void AllIntSpinBox::SetInteger(std::unique_ptr<ILimitedInteger> value)
{
  m_value = std::move(value);
}

AllIntSpinBox::~AllIntSpinBox() = default;

QVariant AllIntSpinBox::value() const
{
  return GetQtVariant(m_value->GetValueAsVariant());
}

QValidator::State AllIntSpinBox::validate(QString &input, int &pos) const
{
  //    (void)pos;

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

  // Try to do a formal conversion through locale
  bool conversion_ok;
  const QVariant converted_value = locale().toLongLong(input, &conversion_ok);
  if (!conversion_ok)
  {
    return QValidator::Invalid;
  }

  //    // Ensure the value is within bounds, but allow out of bounds values as an intermediate
  //    state

  //    const bool within_bounds{converted_value >= m_minimum && converted_value <= m_maximum};
  //    return within_bounds ? QValidator::Acceptable : QValidator::Intermediate;

  return QValidator::Acceptable;
}

void AllIntSpinBox::stepBy(int steps)
{
  if (m_value->StepBy(steps))
  {
    emit valueChanged(GetQtVariant(m_value->GetValueAsVariant()));
  }
}

QAbstractSpinBox::StepEnabled AllIntSpinBox::stepEnabled() const
{
  if (isReadOnly())
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
  m_value->SetValueFromVariant(GetStdVariant(value));
}

void AllIntSpinBox::OnEditingFinished()
{
  auto text_value = text();
  int pos = 0;  // unused
  if (validate(text_value, pos) == QValidator::Acceptable)
  {
    if (m_value->SetValueFromText(text_value.toStdString()))
    {
      emit valueChanged(GetQtVariant(m_value->GetValueAsVariant()));
    }
  }
}

void AllIntSpinBox::updateEdit()
{
  // Propagate new value to the text line editor
  const auto value_text = QString::fromStdString(m_value->GetValueAsText());
  if (value_text != text())
  {
    lineEdit()->setText(value_text);
  }
}

}  // namespace mvvm
