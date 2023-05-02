/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_EDITORS_SELECTABLE_COMBOBOX_EDITOR_H_
#define MVVM_EDITORS_SELECTABLE_COMBOBOX_EDITOR_H_

#include <mvvm/editors/custom_editor.h>
#include <mvvm/viewmodel/custom_variants.h>

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
  void OnModelDataChanged(const QModelIndex&, const QModelIndex&, const QVector<int>&);

  void OnClickedList(const QModelIndex& index);

  void UpdateComponents() override;

private:
  bool eventFilter(QObject* obj, QEvent* event) override;

  void SetConnected(bool isConnected);
  void UpdateBoxLabel();

  bool IsClickToSelect(QObject* obj, QEvent* event) const;
  bool IsClickToExpand(QObject* obj, QEvent* event) const;

  QComboBox* m_box{nullptr};
  WheelEventFilter* m_wheel_event_filter{nullptr};
  QStandardItemModel* m_model{nullptr};
};

}  // namespace mvvm

#endif  // MVVM_EDITORS_SELECTABLE_COMBOBOX_EDITOR_H_
