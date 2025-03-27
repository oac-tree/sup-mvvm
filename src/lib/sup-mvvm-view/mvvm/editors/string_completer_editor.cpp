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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "string_completer_editor.h"

#include <mvvm/core/exceptions.h>

#include <QDebug>
#include <QLineEdit>
#include <QVBoxLayout>

namespace mvvm
{

StringCompleterEditor::StringCompleterEditor(const string_list_func_t &func, QWidget *parent_widget)
    : QWidget(parent_widget), m_string_list_func(func), m_line_edit(new QLineEdit)
{
  if (!m_string_list_func)
  {
    throw RuntimeException("Function is not initialized");
  }

  setAutoFillBackground(true);

  auto layout = new QVBoxLayout;
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_line_edit);
  setLayout(layout);

  SetConnected(true);
}

QVariant StringCompleterEditor::value() const
{
  return m_value;
}

void StringCompleterEditor::setValue(const QVariant &value)
{
  m_value = value;
}

QLineEdit *StringCompleterEditor::GetLineEdit() const
{
  return m_line_edit;
}

void StringCompleterEditor::OnEditingFinished()
{
  qDebug() << "StringCompleterEditor::OnEditingFinished";
}

void StringCompleterEditor::SetConnected(bool is_connected)
{
  if (is_connected)
  {
    connect(m_line_edit, &QLineEdit::editingFinished, this,
            &StringCompleterEditor::OnEditingFinished, Qt::UniqueConnection);
  }
  else
  {
    disconnect(m_line_edit, &QLineEdit::editingFinished, this,
               &StringCompleterEditor::OnEditingFinished);
  }
}

}  // namespace mvvm
