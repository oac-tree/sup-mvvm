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

#include "scientific_spinbox.h"

#include <mvvm/standarditems/editor_constants.h>

#include <QLineEdit>
#include <cmath>
#include <limits>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QRegExp>
#else
#include <QRegularExpression>
#endif

namespace
{
const double upper_switch = 1000;
const double lower_switch = 0.1;
const double min_val = std::numeric_limits<double>::min();
const double max_val = std::numeric_limits<double>::max();

bool useExponentialNotation(double val);
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
  QLocale locale;
  locale.setNumberOptions(QLocale::RejectGroupSeparator);
  m_validator.setLocale(locale);
  m_validator.setNotation(QDoubleValidator::ScientificNotation);

  connect(this, &QAbstractSpinBox::editingFinished, this, &ScientificSpinBox::updateValue);
}

ScientificSpinBox::~ScientificSpinBox() = default;

double ScientificSpinBox::value() const
{
  // return last acceptable input (required for the proper focus-out behaviour)
  double val = toDouble(text(), m_validator, m_min, m_max, m_value);
  return round(val, m_decimals);
}

void ScientificSpinBox::setValue(double val)
{
  double old_val = m_value;
  m_value = round(val, m_decimals);
  updateText();
  if (std::abs(old_val - m_value) > min_val)
    emit valueChanged(m_value);
}

void ScientificSpinBox::updateValue()
{
  double new_val = toDouble(text(), m_validator, m_min, m_max, m_value);
  setValue(new_val);
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
    setValue(m_min);
}

double ScientificSpinBox::maximum() const
{
  return m_max;
}

void ScientificSpinBox::setMaximum(double max)
{
  m_max = max;
  if (m_value > m_max)
    setValue(m_max);
}

void ScientificSpinBox::setDecimals(int val)
{
  if (val <= 0)
    return;
  m_decimals = val;
  setValue(m_value);
}

int ScientificSpinBox::decimals() const
{
  return m_decimals;
}

void ScientificSpinBox::stepBy(int steps)
{
  double new_val = round(m_value + m_step * steps, m_decimals);
  if (inRange(new_val))
    setValue(new_val);
}

QString ScientificSpinBox::toString(double val, int decimal_points)
{
  QString result = useExponentialNotation(val) ? QString::number(val, 'e', decimal_points)
                                               : QString::number(val, 'f', decimal_points);

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
  return result.replace(QRegExp("(\\.?0+)?((e{1}[\\+|-]{1})(0+)?([1-9]{1}.*))?$"), "\\3\\5");
#else
  return result.replace(QRegularExpression("(\\.?0+)?((e{1}[\\+|-]{1})(0+)?([1-9]{1}.*))?$"),
                        "\\3\\5");
#endif
}

double ScientificSpinBox::toDouble(QString text, const QDoubleValidator& validator, double min,
                                   double max, double default_value)
{
  int pos = 0;
  if (validator.validate(text, pos) == QValidator::Acceptable)
  {
    double new_val = validator.locale().toDouble(text);
    if (std::abs(new_val) < min_val)
      new_val = 0.0;
    return new_val >= min && new_val <= max ? new_val : default_value;
  }
  return default_value;
}

double ScientificSpinBox::round(double val, int decimals)
{
  char notation = useExponentialNotation(val) ? 'e' : 'f';
  return QString::number(val, notation, decimals).toDouble();
}

QAbstractSpinBox::StepEnabled ScientificSpinBox::stepEnabled() const
{
  return isReadOnly() ? StepNone : StepUpEnabled | StepDownEnabled;
}

void ScientificSpinBox::updateText()
{
  QString new_text = toString(m_value, m_decimals);
  if (new_text != text())
    lineEdit()->setText(new_text);
}

bool ScientificSpinBox::inRange(double val) const
{
  return val >= m_min && val <= m_max;
}

}  // namespace mvvm

namespace
{
bool useExponentialNotation(double val)
{
  const double abs_val = std::abs(val);

  if (abs_val <= min_val)
    return false;

  return abs_val >= upper_switch || abs_val < lower_switch;
}
}  // namespace
