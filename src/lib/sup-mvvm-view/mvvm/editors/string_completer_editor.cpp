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

#include <QCompleter>
#include <QDebug>
#include <QEvent>
#include <QHeaderView>
#include <QLineEdit>
#include <QStringListModel>
#include <QTreeView>
#include <QVBoxLayout>

namespace mvvm
{

StringCompleterEditor::StringCompleterEditor(const string_list_func_t &func, QWidget *parent_widget)
    : QWidget(parent_widget)
    , m_string_list_func(func)
    , m_line_edit(new QLineEdit)
    , m_completer(new QCompleter(this))
    , m_completer_view(new QTreeView)
    , m_completer_model(new QStringListModel(this))
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
  // setFocusProxy(m_line_edit);

  UpdateCompleterModel();
  SetupCompleter();
  SetConnected(true);

  m_line_edit->installEventFilter(this);
}

QVariant StringCompleterEditor::value() const
{
  return m_value;
}

void StringCompleterEditor::setValue(const QVariant &value)
{
  if (m_value != value)
  {
    m_value = value;
    m_line_edit->setText(value.toString());
    qDebug() << "emitting valueChanged";
    emit valueChanged(m_value);
  }
}

QLineEdit *StringCompleterEditor::GetLineEdit() const
{
  return m_line_edit;
}

QCompleter *StringCompleterEditor::GetCompleter() const
{
  return m_completer;
}

// bool StringCompleterEditor::eventFilter(QObject *object, QEvent *event)
// {
//   if (event->type() == QEvent::FocusIn)
//   {
//     qDebug() << "focusIn" << object << event;
//   }
//   return false;
// }

void StringCompleterEditor::OnEditingFinished()
{
  qDebug() << "StringCompleterEditor::OnEditingFinished" << m_line_edit->text();
  const QVariant new_value(m_line_edit->text());
  if (m_value != new_value)
  {
    m_value = new_value;
    qDebug() << "emitting valueChanged";
    emit valueChanged(new_value);
  }
}

void StringCompleterEditor::UpdateCompleterModel()
{
  m_completer_model->setStringList(m_string_list_func());
}

void StringCompleterEditor::SetupCompleter()
{
  m_completer->setModel(m_completer_model);
  m_completer->setPopup(m_completer_view);
  // m_completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
  m_completer_view->setRootIsDecorated(false);
  m_completer_view->header()->hide();

  m_line_edit->setCompleter(m_completer);
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
