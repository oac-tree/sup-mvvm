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

#include "mvvm/editors/longint_spinbox.h"

#include <mvvm/standarditems/editor_constants.h>

#include <QByteArray>
#include <QLineEdit>
#include <QLocale>
#include <QValidator>
#include <cmath>
#include <limits>

namespace mvvm
{

LongIntSpinBox::LongIntSpinBox(QWidget *parent)
    : QAbstractSpinBox(parent)
    , m_value{INT64_C(0)}
    , m_minimum{std::numeric_limits<qint64>::min()}
    , m_maximum{std::numeric_limits<qint64>::max()}
    , m_step_value{INT64_C(1)}
{
  // Modify locale to omit group separators
  auto current_locale{locale()};
  current_locale.setNumberOptions(QLocale::OmitGroupSeparator);
  setLocale(current_locale);

  // Propagate editor value to internal value
  connect(this, &QAbstractSpinBox::editingFinished, this, &LongIntSpinBox::updateValue);
  // Propagate value changes to editor
  connect(this, &LongIntSpinBox::valueChanged, this, &LongIntSpinBox::updateEdit);
}

LongIntSpinBox::~LongIntSpinBox() = default;

qint64 LongIntSpinBox::value() const
{
  return valueFromText(text());
}

void LongIntSpinBox::setValue(const qint64 value)
{
  // Ensure that the value is within the range
  const qint64 new_value = value < m_minimum ? m_minimum : value < m_maximum ? value : m_maximum;

  // Only update the value if it has changed
  if (new_value != m_value)
  {
    m_value = new_value;
    emit valueChanged(m_value);
  }
}

qint64 LongIntSpinBox::singleStep() const
{
  return m_step_value;
}

void LongIntSpinBox::setSingleStep(const qint64 step_value)
{
  if (step_value >= INT64_C(0))
  {
    m_step_value = step_value;
  }
}

qint64 LongIntSpinBox::minimum() const
{
  return m_minimum;
}

void LongIntSpinBox::setMinimum(const qint64 minimum)
{
  // Set the maximum to the new minimum if the current maximum is less than the new minimum
  setRange(minimum, m_maximum > minimum ? m_maximum : minimum);
}

qint64 LongIntSpinBox::maximum() const
{
  return m_maximum;
}

void LongIntSpinBox::setMaximum(const qint64 maximum)
{
  // Set the minimum to the new maximum if the current minimum is more than the new maximum
  setRange(m_minimum < maximum ? m_minimum : maximum, maximum);
}

void LongIntSpinBox::setRange(const qint64 minimum, const qint64 maximum)
{
  // Do nothing if the range is invalid
  if (minimum > maximum)
  {
    return;
  }

  m_minimum = minimum;
  m_maximum = maximum;

  // Ensure the current value is within the new limits, setValue does the bound checking for us
  setValue(m_value);
}

QValidator::State LongIntSpinBox::validate(QString &input, int &pos) const
{
  (void)pos;

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
  const qint64 converted_value = locale().toLongLong(input, &conversion_ok);
  if (!conversion_ok)
  {
    return QValidator::Invalid;
  }

  // Ensure the value is within bounds, but allow out of bounds values as an intermediate state
  const bool within_bounds{converted_value >= m_minimum && converted_value <= m_maximum};
  return within_bounds ? QValidator::Acceptable : QValidator::Intermediate;
}

void LongIntSpinBox::fixup(QString &input) const
{
  // Convert from text to integer and back to text
  bool conversion_ok;
  const qint64 converted_value = locale().toLongLong(input, &conversion_ok);
  if (conversion_ok)
  {
    input = locale().toString(converted_value);
  }
}

void LongIntSpinBox::stepBy(const int steps)
{
  // setValue does the bound checking for us
  setValue(m_value + (steps * m_step_value));
}

qint64 LongIntSpinBox::valueFromText(const QString &text) const
{
  bool conversion_ok;
  const qint64 converted_value = locale().toLongLong(text, &conversion_ok);
  return conversion_ok ? converted_value : INT64_C(0);
}

QString LongIntSpinBox::textFromValue(const qint64 value) const
{
  return locale().toString(value);
}

QAbstractSpinBox::StepEnabled LongIntSpinBox::stepEnabled() const
{
  if (isReadOnly())
  {
    return StepNone;
  }

  QAbstractSpinBox::StepEnabled step_enabled{StepNone};
  if (m_value < m_maximum)
  {
    step_enabled |= StepUpEnabled;
  }
  if (m_value > m_minimum)
  {
    step_enabled |= StepDownEnabled;
  }
  return step_enabled;
}

void LongIntSpinBox::updateValue()
{
  auto text_value = text();
  int pos = 0;  // unused
  if (validate(text_value, pos) == QValidator::Acceptable)
  {
    // If the value on the editor is valid update the local value
    setValue(valueFromText(text_value));
  }
}

void LongIntSpinBox::updateEdit()
{
  // Propagate new value to the editor
  const auto value_text = textFromValue(m_value);
  if (value_text != text())
  {
    lineEdit()->setText(value_text);
  }
}

}  // namespace mvvm
