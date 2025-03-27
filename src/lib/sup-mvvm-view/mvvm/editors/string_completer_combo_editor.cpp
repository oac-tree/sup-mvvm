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

#include "string_completer_combo_editor.h"

#include <mvvm/core/exceptions.h>

#include <QComboBox>
#include <QDebug>
#include <QLineEdit>
#include <QVBoxLayout>

namespace mvvm
{

StringCompleterComboEditor::StringCompleterComboEditor(const string_list_func_t &func,
                                                       QWidget *parent_widget)
    : QWidget(parent_widget), m_string_list_func(func), m_combo_box(new QComboBox)
{
  if (!m_string_list_func)
  {
    throw RuntimeException("Function is not initialized");
  }

  setAutoFillBackground(true);

  m_combo_box->setEditable(true);
  m_combo_box->setInsertPolicy(QComboBox::NoInsert);

  auto layout = new QVBoxLayout;
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_combo_box);
  setLayout(layout);

  UpdateComboBox();

  SetConnected(true);
}

QVariant StringCompleterComboEditor::value() const
{
  return m_value;
}

void StringCompleterComboEditor::setValue(const QVariant &value)
{
  qDebug() << "StringCompleterComboEditor::setValue" << m_combo_box->currentText() << value.toString();
  if (m_value != value)
  {
    m_value = value;
    m_combo_box->setCurrentText(m_value.toString());
    qDebug() << "emitting valueChanged";
    emit valueChanged(m_value);
  }
}

QComboBox *StringCompleterComboEditor::GetComboBox() const
{
  return m_combo_box;
}

void StringCompleterComboEditor::UpdateComboBox()
{
  m_combo_box->clear();
  auto text_list = m_string_list_func();
  if (!text_list.isEmpty())
  {
    // always empty entry first
    text_list.prepend(QString());
  }
  m_combo_box->insertItems(0, text_list);
}

void StringCompleterComboEditor::OnIndexChanged(int index)
{
  qDebug() << "StringCompleterComboEditor::OnIndexChanged" << index;
}

void StringCompleterComboEditor::OnEditTextChanged(const QString &text)
{
  qDebug() << "StringCompleterComboEditor::OnEditTextChanged" << text;
}

void StringCompleterComboEditor::OnEditingFinished()
{
  qDebug() << "StringCompleterComboEditor::OnEditingFinished";
}

void StringCompleterComboEditor::SetConnected(bool is_connected)
{
  if (is_connected)
  {
    connect(m_combo_box, &QComboBox::editTextChanged, this,
            &StringCompleterComboEditor::OnEditTextChanged, Qt::UniqueConnection);
    connect(m_combo_box, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &StringCompleterComboEditor::OnIndexChanged, Qt::UniqueConnection);
    connect(m_combo_box->lineEdit(), &QLineEdit::editingFinished, this,
            &StringCompleterComboEditor::OnEditingFinished, Qt::UniqueConnection);
  }
  else
  {
    disconnect(m_combo_box, &QComboBox::editTextChanged, this,
               &StringCompleterComboEditor::OnEditTextChanged);
    disconnect(m_combo_box, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
               this, &StringCompleterComboEditor::OnIndexChanged);
    disconnect(m_combo_box->lineEdit(), &QLineEdit::editingFinished, this,
               &StringCompleterComboEditor::OnEditingFinished);
  }
}

}  // namespace mvvm
