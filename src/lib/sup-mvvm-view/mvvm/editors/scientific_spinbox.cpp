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

#include "scientific_spinbox.h"

#include <mvvm/providers/variant_converter.h>
#include <mvvm/standarditems/editor_constants.h>
#include <mvvm/utils/i_limited_float.h>
#include <mvvm/utils/limited_float_helper.h>

#include <QLineEdit>

namespace
{
bool HasFloatCharactersOnly(const std::string& str)
{
  return str.find_first_not_of("0123456789+-.eE") == std::string::npos;
}
}  // namespace

namespace mvvm
{

ScientificSpinBox::ScientificSpinBox(QWidget* parent_widget)
    : QAbstractSpinBox(parent_widget), m_precision(constants::kDefaultDoubleDecimals)
{
  connect(this, &QAbstractSpinBox::editingFinished, this, &ScientificSpinBox::OnEditingFinished);
}

ScientificSpinBox::~ScientificSpinBox() = default;

void ScientificSpinBox::SetLimitedFloat(std::unique_ptr<ILimitedFloat> value)
{
  m_value = std::move(value);
  m_value->SetSingleStep(m_single_step);
  UpdateTextField();
}

ILimitedFloat* ScientificSpinBox::GetLimitedFloat()
{
  return m_value.get();
}

void ScientificSpinBox::SetRange(const variant_t& lower_limit, const variant_t& upper_limit)
{
  m_lower_limit = lower_limit;
  m_upper_limit = upper_limit;
}

void ScientificSpinBox::SetPrecision(std::int32_t precision)
{
  m_precision = precision;
  if (m_value)
  {
    UpdateTextField();
  }
}

std::int32_t ScientificSpinBox::GetPrecision() const
{
  return m_precision;
}

void ScientificSpinBox::SetScientific(bool value)
{
  m_scientific = value;
  if (m_value)
  {
    UpdateTextField();
  }
}

bool ScientificSpinBox::IsScientific() const
{
  return m_scientific;
}

void ScientificSpinBox::SetSingleStep(double step)
{
  m_single_step = step;
  if (m_value)
  {
    m_value->SetSingleStep(step);
  }
}

double ScientificSpinBox::GetSingleStep() const
{
  return m_single_step;
}

QVariant ScientificSpinBox::value() const
{
  if (!m_value)
  {
    return {};
  }

  // typed text not yet processed on editingFinished takes precedence
  if (const auto text_value = text().toStdString(); text_value != GetValueText())
  {
    if (auto parsed_value = m_value->ParseText(text_value); parsed_value.has_value())
    {
      return GetQtVariant(parsed_value.value());
    }
  }

  return GetQtVariant(m_value->GetValueAsVariant());
}

QValidator::State ScientificSpinBox::validate(QString& input, int& pos) const
{
  (void)pos;

  if (input.isEmpty())
  {
    // An empty field means no value for a nullable float, otherwise it is an intermediate state
    return (m_value && m_value->IsNullable()) ? QValidator::Acceptable : QValidator::Intermediate;
  }

  if (!HasFloatCharactersOnly(input.toStdString()))
  {
    return QValidator::Invalid;
  }

  // We are performing final conversion check on board of ILimitedFloat::SetValueFromText.
  // Intermediate input like "1e" is accepted here.

  return QValidator::Acceptable;
}

void ScientificSpinBox::fixup(QString& input) const
{
  (void)input;
}

void ScientificSpinBox::stepBy(int steps)
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

void ScientificSpinBox::CheckNotify()
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

QAbstractSpinBox::StepEnabled ScientificSpinBox::stepEnabled() const
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

void ScientificSpinBox::setValue(const QVariant& value)
{
  if (!m_value)
  {
    SetLimitedFloat(CreateLimitedFloat(GetStdVariant(value), m_lower_limit, m_upper_limit));
    return;
  }

  if (m_value->SetValueFromVariant(GetStdVariant(value)))
  {
    m_cached_value_was_changed = true;
    UpdateTextField();
  }

  CheckNotify();
}

void ScientificSpinBox::OnEditingFinished()
{
  if (!m_value)
  {
    return;
  }

  // the text of the current value keeps the exact value
  if (const auto text_value = text().toStdString(); text_value != GetValueText())
  {
    if (m_value->SetValueFromText(text_value))
    {
      m_cached_value_was_changed = true;
    }
  }

  // invalid text is replaced with the text of the value
  UpdateTextField();
  CheckNotify();
}

void ScientificSpinBox::UpdateTextField()
{
  const auto value_text = QString::fromStdString(GetValueText());
  if (value_text != text())
  {
    lineEdit()->setText(value_text);
    lineEdit()->selectAll();
  }
}

std::string ScientificSpinBox::GetValueText() const
{
  return m_value->GetValueAsText(m_precision, m_scientific);
}

}  // namespace mvvm
