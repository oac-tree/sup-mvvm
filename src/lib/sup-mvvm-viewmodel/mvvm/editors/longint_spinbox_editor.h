/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Rafael Silva (EXT)
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

#ifndef MVVM_EDITORS_LONGINT_SPINBOX_EDITOR_H_
#define MVVM_EDITORS_LONGINT_SPINBOX_EDITOR_H_

#include <mvvm/editors/custom_editor.h>

#include <QtCore>

namespace mvvm
{

class LongIntSpinBox;

//! Custom editor for QVariant based on long integers (64 bits in size).

class MVVM_VIEWMODEL_EXPORT LongIntSpinBoxEditor : public CustomEditor
{
  Q_OBJECT

public:
  explicit LongIntSpinBoxEditor(QWidget* parent = nullptr);

  void SetRange(qint64 minimum, qint64 maximum);
  void SetSingleStep(qint64 step);

  bool IsPersistent() const override;

  int64 GetMinimum() const;

  int64 GetMaximum() const;

private:
  void OnValueChanged(qint64 value);

  void UpdateComponents() override;
  LongIntSpinBox* m_longint_editor{nullptr};
};

}  // namespace mvvm

#endif  // MVVM_EDITORS_LONGINT_SPINBOX_EDITOR_H_
