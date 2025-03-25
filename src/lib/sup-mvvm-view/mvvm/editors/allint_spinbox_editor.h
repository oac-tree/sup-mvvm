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

#ifndef MVVM_EDITORS_ALLINT_SPINBOX_EDITOR_H_
#define MVVM_EDITORS_ALLINT_SPINBOX_EDITOR_H_

#include <mvvm/providers/custom_editor.h>

namespace mvvm
{

class AllIntSpinBox;

//! Custom editor for QVariant based on int family (from int8 to int64).
//! FIXME remove the whole class and just use AllIntSpinBox
class MVVM_VIEWMODEL_EXPORT AllIntSpinBoxEditor : public CustomEditor
{
  Q_OBJECT

public:
  explicit AllIntSpinBoxEditor(QWidget* parent_widget = nullptr);

  bool IsPersistent() const override;

  void SetRange(const variant_t& lower_limit, const variant_t& upper_limit);

  variant_t GetLowerLimit() const;

  variant_t GetUpperLimit() const;

private:
  void OnValueChanged(const QVariant& value);

  void UpdateComponents() override;
  AllIntSpinBox* m_allint_editor{nullptr};
  variant_t m_user_lower_limit;
  variant_t m_user_upper_limit;
};

}  // namespace mvvm

#endif  // MVVM_EDITORS_ALLINT_SPINBOX_EDITOR_H_
