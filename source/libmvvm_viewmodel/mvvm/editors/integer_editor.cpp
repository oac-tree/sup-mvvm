/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/editors/integer_editor.h"

#include <QSpinBox>
#include <QVBoxLayout>
#include <cmath>
#include <stdexcept>

namespace
{
const int max_val = 65536;
const int min_val = -max_val;
}  // namespace

namespace mvvm
{

IntegerEditor::IntegerEditor(QWidget* parent) : CustomEditor(parent), m_int_editor(new QSpinBox)
{
  setAutoFillBackground(true);
  m_int_editor->setFocusPolicy(Qt::StrongFocus);
  m_int_editor->setKeyboardTracking(false);
  m_int_editor->setRange(min_val, max_val);

  auto layout = new QVBoxLayout;
  layout->setMargin(0);
  layout->setSpacing(0);

  layout->addWidget(m_int_editor);

  connect(m_int_editor, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
          [=] { this->onEditingFinished(); });

  setLayout(layout);

  setFocusProxy(m_int_editor);
}

void IntegerEditor::setRange(int minimum, int maximum)
{
  m_int_editor->setRange(minimum, maximum);
}

void IntegerEditor::onEditingFinished()
{
  int new_value = m_int_editor->value();
  if (new_value != GetData().value<int>())
  {
    SetDataIntern(QVariant::fromValue(new_value));
  }
}

void IntegerEditor::UpdateComponents()
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
  if (GetData().type() != QVariant::Int)
#else
  if (GetData().typeId() != QMetaType::Int)
#endif
  {
    throw std::runtime_error("IntegerEditor::UpdateComponents() -> Error. Wrong variant type");
  }

  m_int_editor->setValue(GetData().value<int>());
}

}  // namespace mvvm
