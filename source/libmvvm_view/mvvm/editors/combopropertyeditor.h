// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#ifndef MVVM_EDITORS_COMBOPROPERTYEDITOR_H
#define MVVM_EDITORS_COMBOPROPERTYEDITOR_H

#include "mvvm/editors/customeditor.h"
#include "mvvm/viewmodel/customvariants.h"

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

#endif  // MVVM_EDITORS_COMBOPROPERTYEDITOR_H
