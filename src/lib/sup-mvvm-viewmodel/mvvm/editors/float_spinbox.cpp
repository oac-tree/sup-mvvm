/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
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

#include <mvvm/viewmodel/custom_variants.h>

#include <QDoubleSpinBox>
#include <QVBoxLayout>

namespace mvvm
{

FloatSpinBox::FloatSpinBox(QWidget *parent) : QWidget(parent), m_double_editor(new QDoubleSpinBox)
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

void FloatSpinBox::SetRange(double lower_limit, double upper_limit)
{
  m_lower_limit = lower_limit;
  m_upper_limit = upper_limit;
  m_double_editor->setRange(m_lower_limit, m_upper_limit);
}

double FloatSpinBox::minimum() const
{
  return m_lower_limit;
}

double FloatSpinBox::maximum() const
{
  return m_upper_limit;
}

void FloatSpinBox::OnEditingFinished(double value)
{
  QVariant new_value;

  // since we are using float64 values to edit both, float32 and float64 with the help of
  // QDoubleSpinBox editor, we need to convert double to original QVariant.

  if (utils::GetQtVariantName(m_value) == constants::kFloat64QtTypeName)
  {
    new_value = QVariant::fromValue(value);
  }
  else if (utils::GetQtVariantName(m_value) == constants::kFloat32QtTypeName)
  {
    // editor was instructed already for correct min and max, no harm to downcast
    new_value = QVariant::fromValue(static_cast<mvvm::float32>(value));
  }

  if (new_value.isValid() && new_value != m_value)
  {
    m_value = new_value;
    emit valueChanged(m_value);
  }
}

}  // namespace mvvm
