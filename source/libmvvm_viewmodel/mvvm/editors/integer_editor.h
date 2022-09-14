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

#ifndef MVVM_EDITORS_INTEGER_EDITOR_H_
#define MVVM_EDITORS_INTEGER_EDITOR_H_

#include <mvvm/editors/custom_editor.h>

class QSpinBox;

namespace mvvm
{

//! Custom editor for QVariant based on integer with possibility to set limits.

class MVVM_VIEWMODEL_EXPORT IntegerEditor : public CustomEditor
{
  Q_OBJECT

public:
  explicit IntegerEditor(QWidget* parent = nullptr);

  void SetRange(int minimum, int maximum);

private slots:
  void onEditingFinished();

private:
  void UpdateComponents() override;
  QSpinBox* m_int_editor{nullptr};
};

}  // namespace mvvm

#endif  // MVVM_EDITORS_INTEGER_EDITOR_H_
