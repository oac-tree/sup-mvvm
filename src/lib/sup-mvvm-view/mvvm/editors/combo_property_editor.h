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

#ifndef MVVM_EDITORS_COMBO_PROPERTY_EDITOR_H_
#define MVVM_EDITORS_COMBO_PROPERTY_EDITOR_H_

#include <mvvm/providers/custom_editor.h>
#include <mvvm/viewmodel/custom_variants.h>

class QComboBox;

namespace mvvm
{

/**
 * @brief The ComboPropertyEditor class is a cell editor for QVariant based on ComboProperty.
 */
class MVVM_VIEWMODEL_EXPORT ComboPropertyEditor : public CustomEditor
{
  Q_OBJECT

public:
  explicit ComboPropertyEditor(QWidget* parent_widget = nullptr);

  QSize sizeHint() const override;
  QSize minimumSizeHint() const override;

  /**
   * @brief Returns underlying QComboBox.
   */
  QComboBox* GetComboBox();

protected slots:
  void OnIndexChanged(int index);

private:
  int GetInternIndex();
  void SetConnected(bool is_connected);
  void UpdateComponents() override;

  QComboBox* m_combo_box{nullptr};
};

}  // namespace mvvm

#endif  // MVVM_EDITORS_COMBO_PROPERTY_EDITOR_H_
