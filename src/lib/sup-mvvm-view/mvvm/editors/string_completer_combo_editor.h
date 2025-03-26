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

#include <QVariant>
#include <QWidget>

class QComboBox;

namespace mvvm
{

class StringCompleterComboEditor : public QWidget
{
  Q_OBJECT

  // Thanks to this property model delegates knows how to commit the data using this editor
  Q_PROPERTY(QVariant value MEMBER m_value READ value WRITE setValue NOTIFY valueChanged USER true)

public:
  explicit StringCompleterComboEditor(QWidget* parent_widget = nullptr);

  QVariant value() const;

  void setValue(const QVariant& value);

signals:
  void valueChanged(const QVariant& value);

private:
  void OnEditingFinished(double value);

  QVariant m_value;
  QComboBox* m_box{nullptr};
};

}  // namespace mvvm

#endif  // MVVM_EDITORS_STRING_COMPLETER_COMBO_EDITOR_H_
