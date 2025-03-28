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

#ifndef MVVM_EDITORS_STRING_COMPLETER_COMBO_EDITOR_H_
#define MVVM_EDITORS_STRING_COMPLETER_COMBO_EDITOR_H_

#include <QStringList>
#include <QVariant>
#include <QWidget>
#include <functional>

class QComboBox;

namespace mvvm
{

class StringCompleterComboEditor : public QWidget
{
  Q_OBJECT

  // Thanks to this property model delegates knows how to commit the data using this editor
  Q_PROPERTY(QVariant value MEMBER m_value READ value WRITE setValue NOTIFY valueChanged USER true)

public:
  using string_list_func_t = std::function<QStringList()>;

  explicit StringCompleterComboEditor(const string_list_func_t& func,
                                      QWidget* parent_widget = nullptr);

  QVariant value() const;

  void setValue(const QVariant& value);

  /**
   * @brief Returns underlying combo box.
   */
  QComboBox* GetComboBox() const;

  bool eventFilter(QObject* object, QEvent* event) override;

  /**
   * @brief Returns string list currently populating combo.
   */
  QStringList GetStringList();

signals:
  void valueChanged(const QVariant& value);

private:
  void UpdateComboBox();
  void OnIndexChanged(int index);
  void OnEditTextChanged(const QString& text);
  void OnEditingFinished();
  void SetConnected(bool isConnected);

  QVariant m_value;
  string_list_func_t m_string_list_func;
  QComboBox* m_combo_box{nullptr};
};

}  // namespace mvvm

#endif  // MVVM_EDITORS_STRING_COMPLETER_COMBO_EDITOR_H_
