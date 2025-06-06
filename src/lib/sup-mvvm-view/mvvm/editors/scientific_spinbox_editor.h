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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_EDITORS_SCIENTIFIC_SPINBOX_EDITOR_H_
#define MVVM_EDITORS_SCIENTIFIC_SPINBOX_EDITOR_H_

#include <mvvm/providers/custom_editor.h>

namespace mvvm
{

class ScientificSpinBox;

//! Custom editor for QVariant based on double with scientific notation support.

class MVVM_VIEWMODEL_EXPORT ScientificSpinBoxEditor : public CustomEditor
{
  Q_OBJECT

public:
  explicit ScientificSpinBoxEditor(QWidget* parent_widget = nullptr);

  void SetRange(double minimum, double maximum);
  void SetDecimals(int decimals);
  void SetSingleStep(double step);

  bool IsPersistent() const override;

private:
  void OnEditingFinished();

  void UpdateComponents() override;
  ScientificSpinBox* m_double_editor{nullptr};
};

}  // namespace mvvm

#endif  // MVVM_EDITORS_SCIENTIFIC_SPINBOX_EDITOR_H_
