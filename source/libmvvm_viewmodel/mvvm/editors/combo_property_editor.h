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

#ifndef MVVM_EDITORS_COMBO_PROPERTY_EDITOR_H_
#define MVVM_EDITORS_COMBO_PROPERTY_EDITOR_H_

#include <mvvm/editors/custom_editor.h>
#include <mvvm/viewmodel/custom_variants.h>

class QComboBox;

namespace mvvm
{

//! Custom editor for QVariant based on ComboProperty.

class MVVM_VIEWMODEL_EXPORT ComboPropertyEditor : public CustomEditor
{
  Q_OBJECT

public:
  explicit ComboPropertyEditor(QWidget* parent = nullptr);

  QSize sizeHint() const override;
  QSize minimumSizeHint() const override;

  bool IsPersistent() const override;

protected slots:
  virtual void OnIndexChanged(int index);

private:
  std::vector<std::string> GetInternLabels();
  int GetInternIndex();
  void SetConnected(bool isConnected);
  void UpdateComponents() override;

  QComboBox* m_box;
};

}  // namespace mvvm

#endif  // MVVM_EDITORS_COMBO_PROPERTY_EDITOR_H_
