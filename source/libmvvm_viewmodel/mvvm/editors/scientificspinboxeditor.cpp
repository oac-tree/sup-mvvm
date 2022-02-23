/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/editors/scientificspinboxeditor.h"

#include "mvvm/editors/scientificspinbox.h"
#include "mvvm/utils/numericutils.h"

#include <QVBoxLayout>
#include <stdexcept>

namespace mvvm
{

ScientificSpinBoxEditor::ScientificSpinBoxEditor(QWidget* parent)
    : CustomEditor(parent), m_double_editor(new ScientificSpinBox)
{
  setAutoFillBackground(true);
  setFocusPolicy(Qt::StrongFocus);
  m_double_editor->setFocusPolicy(Qt::StrongFocus);
  m_double_editor->setKeyboardTracking(false);

  auto layout = new QVBoxLayout;
  layout->setMargin(0);
  layout->setSpacing(0);

  layout->addWidget(m_double_editor);

  connect(m_double_editor, &ScientificSpinBox::valueChanged, [=] { this->OnEditingFinished(); });

  setLayout(layout);

  setFocusProxy(m_double_editor);
}

void ScientificSpinBoxEditor::SetRange(double minimum, double maximum)
{
  m_double_editor->setMinimum(minimum);
  m_double_editor->setMaximum(maximum);
}

void ScientificSpinBoxEditor::SetDecimals(int decimals)
{
  m_double_editor->setDecimals(decimals);
}

void ScientificSpinBoxEditor::SetSingleStep(double step)
{
  m_double_editor->setSingleStep(step);
}

bool ScientificSpinBoxEditor::IsPersistent() const
{
  return true;
}

void ScientificSpinBoxEditor::OnEditingFinished()
{
  double new_value = m_double_editor->value();

  if (!utils::AreAlmostEqual(new_value, GetData().value<double>()))
  {
    SetDataIntern(QVariant::fromValue(new_value));
  }
}

void ScientificSpinBoxEditor::UpdateComponents()
{
  if (GetData().type() != QVariant::Double)
  {
    throw std::runtime_error(
        "ScientificSpinBoxEditor::update_components() -> Error. Wrong variant type");
  }

  m_double_editor->setValue(GetData().value<double>());
}

}  // namespace mvvm
