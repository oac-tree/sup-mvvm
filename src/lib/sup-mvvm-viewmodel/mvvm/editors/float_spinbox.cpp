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

#include "float_spinbox.h"

#include <mvvm/utils/numeric_utils.h>

#include <QDoubleSpinBox>
#include <QVBoxLayout>
#include <stdexcept>

namespace mvvm
{

FloatSpinBox::FloatSpinBox(QWidget *parent)
    : QAbstractSpinBox(parent), m_double_editor(new QDoubleSpinBox)
{
  auto layout = new QVBoxLayout;
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  layout->addWidget(m_double_editor);

  connect(m_double_editor, qOverload<double>(&QDoubleSpinBox::valueChanged), this,
          &FloatSpinBox::OnEditingFinished);

  setLayout(layout);

  setFocusProxy(m_double_editor);
}

void FloatSpinBox::setDecimals(int prec)
{
  m_double_editor->setDecimals(prec);
}

QVariant FloatSpinBox::value() const
{
  return m_value;
}

void FloatSpinBox::setValue(const QVariant &value)
{
  if (m_value != value)
  {
    m_value = value;
    m_double_editor->setValue(value.value<double>());
  }
}

void FloatSpinBox::SetRange(const QVariant &lower_limit, const QVariant &upper_limit)
{
  m_lower_limit = lower_limit;
  m_upper_limit = upper_limit;
}

void FloatSpinBox::stepBy(int steps)
{
  return m_double_editor->stepBy(steps);
}

QValidator::State FloatSpinBox::validate(QString &str, int &pos) const
{
  return m_double_editor->validate(str, pos);
}

void FloatSpinBox::fixup(QString &str) const
{
  return m_double_editor->fixup(str);
}

void FloatSpinBox::OnEditingFinished(double value)
{
  m_value = QVariant(value);
  emit valueChanged(m_value);
}

}  // namespace mvvm
