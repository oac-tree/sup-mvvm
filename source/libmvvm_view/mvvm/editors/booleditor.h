// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#ifndef MVVM_EDITORS_BOOLEDITOR_H
#define MVVM_EDITORS_BOOLEDITOR_H

#include "mvvm/editors/customeditor.h"

class QCheckBox;

namespace mvvm
{

//! Custom editor for QVariant based on bool values.
//! Contains fancy check box and true/false label.

class MVVM_VIEWMODEL_EXPORT BoolEditor : public CustomEditor
{
  Q_OBJECT

public:
  explicit BoolEditor(QWidget* parent = nullptr);

  bool IsPersistent() const override;

private:
  void OnCheckBoxChange(bool value);

  void UpdateComponents() override;
  QCheckBox* m_checkBox;
};

}  // namespace mvvm

#endif  // MVVM_EDITORS_BOOLEDITOR_H
