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

#ifndef MVVM_EDITORS_BOOLEAN_EDITOR_H_
#define MVVM_EDITORS_BOOLEAN_EDITOR_H_

#include <mvvm/providers/custom_editor.h>

class QCheckBox;

namespace mvvm
{

//! Custom editor for QVariant based on bool values.
//! Contains fancy check box and true/false label.

class MVVM_VIEWMODEL_EXPORT BoolEditor : public CustomEditor
{
  Q_OBJECT

public:
  explicit BoolEditor(QWidget* parent_widget = nullptr);

private:
  void OnCheckBoxChange(bool value);

  void UpdateComponents() override;
  QCheckBox* m_check_box{nullptr};
};

}  // namespace mvvm

#endif  // MVVM_EDITORS_BOOLEAN_EDITOR_H_
