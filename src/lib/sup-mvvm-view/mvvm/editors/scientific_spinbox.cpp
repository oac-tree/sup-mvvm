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

#include <mvvm/standarditems/editor_constants.h>
#include <mvvm/utils/string_utils.h>

#include <QLineEdit>
#include <cmath>
#include <limits>

namespace
{
constexpr double min_val = std::numeric_limits<double>::min();
constexpr double max_val = std::numeric_limits<double>::max();
}  // namespace

namespace mvvm
{

ScientificSpinBox::ScientificSpinBox(QWidget* parent_widget)
    : QAbstractSpinBox(parent_widget)
    , m_value(0.0)
    , m_min(-max_val)
    , m_max(max_val)
    , m_step(1.0)
    , m_decimals(constants::kDefaultDoubleDecimals)
{
  connect(this, &QAbstractSpinBox::editingFinished, this, &ScientificSpinBox::updateValue);
}

ScientificSpinBox::~ScientificSpinBox() = default;

double ScientificSpinBox::value() const
{
  // return last acceptable input (required for the proper focus-out behaviour)
  return valueFromText(text());
}

void ScientificSpinBox::setValue(double val)
{
  const double old_val = m_value;
  m_value = val;
  updateText();
  if (std::abs(old_val - m_value) > min_val)
  {
    emit valueChanged(m_value);
  }
}

void ScientificSpinBox::updateValue()
{
  setValue(valueFromText(text()));
}

double ScientificSpinBox::singleStep() const
{
  return m_step;
}

void ScientificSpinBox::setSingleStep(double step)
{
  m_step = step;
}

double ScientificSpinBox::minimum() const
{
  return m_min;
}

void ScientificSpinBox::setMinimum(double min)
{
  m_min = min;
  if (m_value < m_min)
  {
    setValue(m_min);
  }
}

double ScientificSpinBox::maximum() const
{
  return m_max;
}

void ScientificSpinBox::setMaximum(double max)
{
  m_max = max;
  if (m_value > m_max)
  {
    setValue(m_max);
  }
}

void ScientificSpinBox::setDecimals(int val)
{
  if (val <= 0)
  {
    return;
  }
  m_decimals = val;
  updateText();
}

int ScientificSpinBox::decimals() const
{
  return m_decimals;
}

void ScientificSpinBox::stepBy(int steps)
{
  const double new_val = m_value + (m_step * steps);
  if (inRange(new_val))
  {
    setValue(new_val);
  }
}

QAbstractSpinBox::StepEnabled ScientificSpinBox::stepEnabled() const
{
  return isReadOnly() ? StepNone : StepUpEnabled | StepDownEnabled;
}

void ScientificSpinBox::updateText()
{
  const QString new_text =
      QString::fromStdString(utils::ScientificDoubleToString(m_value, m_decimals));
  if (new_text != text())
  {
    lineEdit()->setText(new_text);
  }
}

bool ScientificSpinBox::inRange(double val) const
{
  return val >= m_min && val <= m_max;
}

double ScientificSpinBox::valueFromText(const QString& text) const
{
  const auto str = text.toStdString();
  if (str == utils::ScientificDoubleToString(m_value, m_decimals))
  {
    // unchanged text keeps the exact value
    return m_value;
  }

  const auto parsed_value = utils::StringToDouble(str);
  return (parsed_value.has_value() && inRange(parsed_value.value())) ? parsed_value.value()
                                                                     : m_value;
}

}  // namespace mvvm
