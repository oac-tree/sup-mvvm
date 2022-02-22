// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#ifndef MVVM_EDITORS_SELECTABLECOMBOBOXEDITOR_H
#define MVVM_EDITORS_SELECTABLECOMBOBOXEDITOR_H

#include "mvvm/editors/customeditor.h"
#include "mvvm/viewmodel/customvariants.h"

class QComboBox;
class QStandardItemModel;

namespace mvvm
{

class WheelEventFilter;

//! Adds multi-selection capabilities to QComboBox.

class MVVM_VIEWMODEL_EXPORT SelectableComboBoxEditor : public CustomEditor
{
  Q_OBJECT

public:
  explicit SelectableComboBoxEditor(QWidget* parent = nullptr);

  QSize sizeHint() const override;
  QSize minimumSizeHint() const override;

  bool IsPersistent() const override;

protected:
  void onModelDataChanged(const QModelIndex&, const QModelIndex&, const QVector<int>&);

  void onClickedList(const QModelIndex& index);

  void UpdateComponents() override;

private:
  bool eventFilter(QObject* obj, QEvent* event) override;
  void setConnected(bool isConnected);
  void updateBoxLabel();

  bool isClickToSelect(QObject* obj, QEvent* event) const;
  bool isClickToExpand(QObject* obj, QEvent* event) const;

  QComboBox* m_box{nullptr};
  WheelEventFilter* m_wheelEventFilter{nullptr};
  QStandardItemModel* m_model{nullptr};
};

}  // namespace mvvm

#endif  // MVVM_EDITORS_SELECTABLECOMBOBOXEDITOR_H
