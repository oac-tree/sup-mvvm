/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#ifndef MVVM_EDITORS_NULLABLE_BOOL_EDITOR_H_
#define MVVM_EDITORS_NULLABLE_BOOL_EDITOR_H_

#include <mvvm/viewmodel/custom_editor.h>

class QComboBox;

namespace mvvm
{

/**
 * @brief The NullableBoolEditor class is a cell editor for a bool which can have no value.
 *
 * @details A combo with three entries: the placeholder text for no value, "True" and "False". No
 * value is an invalid QVariant.
 */
class NullableBoolEditor : public CustomEditor
{
  Q_OBJECT

public:
  explicit NullableBoolEditor(QWidget* parent_widget = nullptr);

  /**
   * @brief Sets the text of the entry for no value.
   */
  void SetPlaceholderText(const QString& text);

  /**
   * @brief Returns the text of the entry for no value.
   */
  QString GetPlaceholderText() const;

  /**
   * @brief Returns underlying QComboBox.
   */
  QComboBox* GetComboBox();

private:
  void OnIndexChanged(int index);
  void UpdateComponents() override;

  QComboBox* m_combo_box{nullptr};
};

}  // namespace mvvm

#endif  // MVVM_EDITORS_NULLABLE_BOOL_EDITOR_H_
