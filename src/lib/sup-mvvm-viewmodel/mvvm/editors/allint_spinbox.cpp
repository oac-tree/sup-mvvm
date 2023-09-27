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

#include "allint_spinbox.h"

#include <mvvm/standarditems/editor_constants.h>
#include <mvvm/utils/limited_integer.h>
#include <mvvm/viewmodel/variant_converter.h>

#include <QEvent>
#include <QLineEdit>
#include <QDebug>

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
  qDebug() << "SetInteger() 1.1";
  m_value = std::move(value);
  updateEdit();
//  emit valueChanged(GetQtVariant(m_value->GetValueAsVariant()));
  qDebug() << "SetInteger() 1.2";
}

AllIntSpinBox::~AllIntSpinBox() = default;

QVariant AllIntSpinBox::value() const
{
  return GetQtVariant(m_value->GetValueAsVariant());
}

QValidator::State AllIntSpinBox::validate(QString &input, int &pos) const
{
  qDebug() << "validate() 1.1";
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

  qDebug() << "validate() 1.2";
  // Try to do a formal conversion through locale
  bool conversion_ok;
  const QVariant converted_value = locale().toLongLong(input, &conversion_ok);
  if (!conversion_ok)
  {
    return QValidator::Invalid;
  }

  qDebug() << "validate() 1.3";
  return QValidator::Acceptable;
}

void AllIntSpinBox::stepBy(int steps)
{
  qDebug() << "stepBy() 1.1" << steps << GetQtVariant(m_value->GetValueAsVariant());
  if (m_value->StepBy(steps))
  {
    qDebug() << "stepBy() 1.2" << GetQtVariant(m_value->GetValueAsVariant());
//    emit valueChanged(GetQtVariant(m_value->GetValueAsVariant()));
    updateEdit();
    qDebug() << "stepBy() 1.3";
    qDebug() << " ";
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
  qDebug() << "setValue() 1.1";
  m_value->SetValueFromVariant(GetStdVariant(value));
  emit valueChanged(GetQtVariant(m_value->GetValueAsVariant()));
  qDebug() << "setValue() 1.2";
}

void AllIntSpinBox::OnEditingFinished()
{  
  auto text_value = text();
  qDebug() << "OnEditingFinished() 1.1" << text_value;
  int pos = 0;  // unused
  if (validate(text_value, pos) == QValidator::Acceptable)
  {
    qDebug() << "   OnEditingFinished() 1.2";
    if (m_value->SetValueFromText(text_value.toStdString()))
    {
      qDebug() << "   OnEditingFinished() 1.3";
      emit valueChanged(GetQtVariant(m_value->GetValueAsVariant()));
    }
  }
  qDebug() << "   OnEditingFinished() 1.4";
}

void AllIntSpinBox::updateEdit()
{
  qDebug() << "updateEdit() 1.1";
  // Propagate new value to the text line editor
  const auto value_text = QString::fromStdString(m_value->GetValueAsText());
  qDebug() << "   updateEdit() 1.2" << value_text << "text()" << text() << GetQtVariant(m_value->GetValueAsVariant());
  if (value_text != text())
  {
    qDebug() << "   updateEdit() 1.3";
    lineEdit()->setText(value_text);
    qDebug() << "   updateEdit() 1.4";
  }
}

}  // namespace mvvm
