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

#ifndef MVVM_EDITORS_STRING_COMPLETER_EDITOR_H_
#define MVVM_EDITORS_STRING_COMPLETER_EDITOR_H_

#include <QStringList>
#include <QVariant>
#include <QWidget>
#include <functional>

class QLineEdit;
class QCompleter;
class QTreeView;
class QStringListModel;

namespace mvvm
{

class StringCompleterEditor : public QWidget
{
  Q_OBJECT

  // thanks to this property model delegates knows how to commit the data using this editor
  Q_PROPERTY(QVariant value MEMBER m_value READ value WRITE setValue NOTIFY valueChanged USER true)

public:
  using string_list_func_t = std::function<QStringList()>;

  explicit StringCompleterEditor(const string_list_func_t& func, QWidget* parent_widget = nullptr);

  QVariant value() const;

  void setValue(const QVariant& value);

  /**
   * @brief Returns underlying line edit widget.
   */
  QLineEdit* GetLineEdit() const;

  /**
   * @brief Returns underlying string completer.
   */
  QCompleter* GetCompleter() const;

signals:
  void valueChanged(const QVariant& value);

private:
  void OnEditingFinished();
  void UpdateCompleterModel();
  void SetupCompleter();
  void SetConnected(bool isConnected);

  QVariant m_value;
  string_list_func_t m_string_list_func;
  QLineEdit* m_line_edit{nullptr};
  QCompleter* m_completer{nullptr};
  QTreeView* m_completer_view{nullptr};
  QStringListModel* m_completer_model{nullptr};
};

}  // namespace mvvm

#endif  // MVVM_EDITORS_STRING_COMPLETER_EDITOR_H_
