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

#ifndef MVVM_EDITORS_SELECTABLE_COMBOBOX_EDITOR_H_
#define MVVM_EDITORS_SELECTABLE_COMBOBOX_EDITOR_H_

#include <mvvm/editors/custom_editor.h>
#include <mvvm/viewmodel/custom_variants.h>

class QComboBox;
class QStandardItemModel;

namespace mvvm
{

class WheelEventFilter;

/**
 * @brief The ComboPropertyEditor class is a cell editor for QVariant based on ComboProperty
 * with the possibility of multiple selections.
 */
class MVVM_VIEWMODEL_EXPORT SelectableComboBoxEditor : public CustomEditor
{
  Q_OBJECT

public:
  explicit SelectableComboBoxEditor(QWidget* parent_widget = nullptr);

  QSize sizeHint() const override;
  QSize minimumSizeHint() const override;

  bool IsPersistent() const override;

protected:
  /**
   * @brief Propagates check state from the model to ComboProperty.
   */
  void OnModelDataChanged(const QModelIndex& top_left, const QModelIndex& bottom_right,
                          const QVector<int>& roles);

  /**
   * @brief Processes press event in QComboBox's underlying list view.
   */
  void OnClickedList(const QModelIndex& index);

  void UpdateComponents() override;

private:
  /**
   * @brief Handles mouse clicks on QComboBox elements.
   */
  bool eventFilter(QObject* obj, QEvent* event) override;

  void SetConnected(bool is_connected);

  /**
   * @brief Updates text on QComboBox with the label provided by combo property.
   */
  void UpdateBoxLabel();

  bool IsClickToSelect(QObject* obj, QEvent* event) const;
  bool IsClickToExpand(QObject* obj, QEvent* event) const;

  QComboBox* m_box{nullptr};
  WheelEventFilter* m_wheel_event_filter{nullptr};
  QStandardItemModel* m_model{nullptr};
};

}  // namespace mvvm

#endif  // MVVM_EDITORS_SELECTABLE_COMBOBOX_EDITOR_H_
