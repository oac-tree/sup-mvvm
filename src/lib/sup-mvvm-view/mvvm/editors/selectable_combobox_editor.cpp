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

// ----------------------------------------------------------------------------
// https://stackoverflow.com/questions/8422760/combobox-of-checkboxes
// https://stackoverflow.com/questions/21186779/catch-mouse-button-pressed-signal-from-qcombobox-popup-menu
// https://gist.github.com/mistic100/c3b7f3eabc65309687153fe3e0a9a720
// ----------------------------------------------------------------------------

#include "selectable_combobox_editor.h"

#include "custom_event_filters.h"

#include <mvvm/model/combo_property.h>
#include <mvvm/utils/container_utils.h>

#include <QAbstractItemView>
#include <QComboBox>
#include <QLineEdit>
#include <QMouseEvent>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QVBoxLayout>

namespace mvvm
{

/**
 * @brief The QCheckListStyledItemDelegate class provides custom style delegate for QComboBox to
 * allow checkboxes.
 */
class QCheckListStyledItemDelegate : public QStyledItemDelegate
{
public:
  explicit QCheckListStyledItemDelegate(QObject* parent = nullptr) : QStyledItemDelegate(parent) {}

  void paint(QPainter* painter, const QStyleOptionViewItem& option,
             const QModelIndex& index) const override
  {
    auto styleOption = const_cast<QStyleOptionViewItem&>(option);
    styleOption.showDecorationSelected = false;
    QStyledItemDelegate::paint(painter, styleOption, index);
  }
};

// -------------------------------------------------------------------------------------------------
// SelectableComboBoxEditor
// -------------------------------------------------------------------------------------------------

SelectableComboBoxEditor::SelectableComboBoxEditor(QWidget* parent_widget)
    : CustomEditor(parent_widget)
    , m_combo_box(new QComboBox)
    , m_wheel_event_filter(new WheelEventFilter(this))
    , m_model(new QStandardItemModel(this))
{
  setAutoFillBackground(true);
  setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

  m_combo_box->installEventFilter(m_wheel_event_filter);
  m_combo_box->view()->viewport()->installEventFilter(this);

  // Editable mode will be used to have None/Multiple labels on top
  m_combo_box->setEditable(true);
  m_combo_box->lineEdit()->setReadOnly(true);
  m_combo_box->lineEdit()->installEventFilter(this);
  connect(m_combo_box->lineEdit(), &QLineEdit::selectionChanged, m_combo_box->lineEdit(),
          &QLineEdit::deselect);

  // transforms ordinary combo box into check list
  m_combo_box->setItemDelegate(new QCheckListStyledItemDelegate(this));
  m_combo_box->setModel(m_model);

  auto layout = new QVBoxLayout;
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_combo_box);
  setLayout(layout);
  SetConnected(true);
}

QSize SelectableComboBoxEditor::sizeHint() const
{
  return m_combo_box->sizeHint();
}

QSize SelectableComboBoxEditor::minimumSizeHint() const
{
  return m_combo_box->minimumSizeHint();
}

bool SelectableComboBoxEditor::IsPersistent() const
{
  return true;
}

QComboBox* SelectableComboBoxEditor::GetComboBox()
{
  return m_combo_box;
}

void SelectableComboBoxEditor::OnModelDataChanged(const QModelIndex& top_left,
                                                  const QModelIndex& bottom_right,
                                                  const QVector<int>& roles)
{
  (void)bottom_right;

#if QT_VERSION > QT_VERSION_CHECK(5, 9, 0)
  // for older versions this role is always empty
  if (!roles.contains(Qt::CheckStateRole))
  {
    return;
  }
#endif

  auto item = m_model->itemFromIndex(top_left);
  if (!item)
  {
    return;
  }

  auto comboProperty = GetData().value<ComboProperty>();
  auto state = item->checkState() == Qt::Checked;
  comboProperty.SetSelected(top_left.row(), state);

  UpdateBoxLabel();
  SetDataIntern(QVariant::fromValue<ComboProperty>(comboProperty));
}

void SelectableComboBoxEditor::OnClickedList(const QModelIndex& index)
{
  if (auto item = m_model->itemFromIndex(index))
  {
    auto state = item->checkState() == Qt::Checked ? Qt::Unchecked : Qt::Checked;
    item->setCheckState(state);
  }
}

void SelectableComboBoxEditor::UpdateComponents()
{
  if (!GetData().canConvert<ComboProperty>())
  {
    return;
  }

  auto property = GetData().value<ComboProperty>();

  SetConnected(false);
  m_model->clear();

  auto labels = property.GetValues();
  auto selected_indices = property.GetSelectedIndices();

  for (size_t i = 0; i < labels.size(); ++i)
  {
    auto item = new QStandardItem(QString::fromStdString(labels[i]));
    m_model->invisibleRootItem()->appendRow(item);
    item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    item->setCheckable(true);

    auto state = utils::Contains(selected_indices, i) ? Qt::Checked : Qt::Unchecked;
    item->setData(state, Qt::CheckStateRole);
    if (i < property.GetToolTips().size())
    {
      item->setData(QString::fromStdString(property.GetToolTips().at(i)), Qt::ToolTipRole);
    }
  }

  SetConnected(true);
  UpdateBoxLabel();
}

bool SelectableComboBoxEditor::eventFilter(QObject* obj, QEvent* event)
{
  if (IsClickToSelect(obj, event))
  {
    // Handles mouse clicks on QListView when it is expanded from QComboBox
    // 1) Prevents list from closing while selecting items.
    // 2) Correctly calculates underlying model index when mouse is over check box style
    // element.
    const auto mouse_event = dynamic_cast<const QMouseEvent*>(event);
    auto index = m_combo_box->view()->indexAt(mouse_event->pos());
    OnClickedList(index);
    return true;
  }

  if (IsClickToExpand(obj, event))
  {
    // Expands box when clicking on None/Multiple label
    m_combo_box->showPopup();
    return true;
  }

  return QObject::eventFilter(obj, event);  // propagate to the parent class
}

void SelectableComboBoxEditor::SetConnected(bool is_connected)
{
  if (is_connected)
  {
    connect(m_model, &QStandardItemModel::dataChanged, this,
            &SelectableComboBoxEditor::OnModelDataChanged);
  }
  else
  {
    disconnect(m_model, &QStandardItemModel::dataChanged, this,
               &SelectableComboBoxEditor::OnModelDataChanged);
  }
}

void SelectableComboBoxEditor::UpdateBoxLabel()
{
  auto combo = GetData().value<ComboProperty>();
  m_combo_box->setCurrentText(QString::fromStdString(combo.GetLabel()));
}

bool SelectableComboBoxEditor::IsClickToSelect(QObject* obj, QEvent* event) const
{
  return obj == m_combo_box->view()->viewport() && event->type() == QEvent::MouseButtonRelease;
}

bool SelectableComboBoxEditor::IsClickToExpand(QObject* obj, QEvent* event) const
{
  return obj == m_combo_box->lineEdit() && event->type() == QEvent::MouseButtonRelease;
}

}  // namespace mvvm
