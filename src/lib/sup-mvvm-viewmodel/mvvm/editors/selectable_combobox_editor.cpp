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

// ----------------------------------------------------------------------------
// https://stackoverflow.com/questions/8422760/combobox-of-checkboxes
// https://stackoverflow.com/questions/21186779/catch-mouse-button-pressed-signal-from-qcombobox-popup-menu
// https://gist.github.com/mistic100/c3b7f3eabc65309687153fe3e0a9a720
// ----------------------------------------------------------------------------

#include "mvvm/editors/selectable_combobox_editor.h"

#include <mvvm/editors/custom_event_filters.h>
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

//! Provides custom style delegate for QComboBox to allow checkboxes.

class QCheckListStyledItemDelegate : public QStyledItemDelegate
{
public:
  QCheckListStyledItemDelegate(QObject* parent = nullptr) : QStyledItemDelegate(parent) {}

  void paint(QPainter* painter, const QStyleOptionViewItem& option,
             const QModelIndex& index) const override
  {
    auto styleOption = const_cast<QStyleOptionViewItem&>(option);
    styleOption.showDecorationSelected = false;
    QStyledItemDelegate::paint(painter, styleOption, index);
  }
};

// ----------------------------------------------------------------------------

SelectableComboBoxEditor::SelectableComboBoxEditor(QWidget* parent)
    : CustomEditor(parent)
    , m_box(new QComboBox)
    , m_wheel_event_filter(new WheelEventFilter(this))
    , m_model(new QStandardItemModel(this))
{
  setAutoFillBackground(true);
  setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

  m_box->installEventFilter(m_wheel_event_filter);
  m_box->view()->viewport()->installEventFilter(this);

  // Editable mode will be used to have None/Multiple labels on top
  m_box->setEditable(true);
  m_box->lineEdit()->setReadOnly(true);
  m_box->lineEdit()->installEventFilter(this);
  connect(m_box->lineEdit(), &QLineEdit::selectionChanged, m_box->lineEdit(), &QLineEdit::deselect);

  // transforms ordinary combo box into check list
  m_box->setItemDelegate(new QCheckListStyledItemDelegate(this));
  m_box->setModel(m_model);

  auto layout = new QVBoxLayout;
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_box);
  setLayout(layout);
  SetConnected(true);
}

QSize SelectableComboBoxEditor::sizeHint() const
{
  return m_box->sizeHint();
}

QSize SelectableComboBoxEditor::minimumSizeHint() const
{
  return m_box->minimumSizeHint();
}

bool SelectableComboBoxEditor::IsPersistent() const
{
  return true;
}

//! Propagate check state from the model to ComboProperty.

void SelectableComboBoxEditor::OnModelDataChanged(const QModelIndex& topLeft, const QModelIndex&,
                                                  const QVector<int>& roles)
{
#if QT_VERSION > QT_VERSION_CHECK(5, 9, 0)
  // for older versions this role is always empty
  if (!roles.contains(Qt::CheckStateRole))
  {
    return;
  }
#endif

  auto item = m_model->itemFromIndex(topLeft);
  if (!item)
  {
    return;
  }

  auto comboProperty = GetData().value<ComboProperty>();
  auto state = item->checkState() == Qt::Checked ? true : false;
  comboProperty.SetSelected(topLeft.row(), state);

  UpdateBoxLabel();
  SetDataIntern(QVariant::fromValue<ComboProperty>(comboProperty));
}

//! Processes press event in QComboBox's underlying list view.

void SelectableComboBoxEditor::OnClickedList(const QModelIndex& index)
{
  if (auto item = m_model->itemFromIndex(index))
  {
    auto state = item->checkState() == Qt::Checked ? Qt::Unchecked : Qt::Checked;
    item->setCheckState(state);
  }
}

//! Handles mouse clicks on QComboBox elements.

bool SelectableComboBoxEditor::eventFilter(QObject* obj, QEvent* event)
{
  if (IsClickToSelect(obj, event))
  {
    // Handles mouse clicks on QListView when it is expanded from QComboBox
    // 1) Prevents list from closing while selecting items.
    // 2) Correctly calculates underlying model index when mouse is over check box style
    // element.
    const auto mouseEvent = static_cast<const QMouseEvent*>(event);
    auto index = m_box->view()->indexAt(mouseEvent->pos());
    OnClickedList(index);
    return true;
  }

  if (IsClickToExpand(obj, event))
  {
    // Expands box when clicking on None/Multiple label
    m_box->showPopup();
    return true;
  }
  else
  {
    // Propagate to the parent class.
    return QObject::eventFilter(obj, event);
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
  auto selectedIndices = property.GetSelectedIndices();

  for (size_t i = 0; i < labels.size(); ++i)
  {
    auto item = new QStandardItem(QString::fromStdString(labels[i]));
    m_model->invisibleRootItem()->appendRow(item);
    item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    item->setCheckable(true);

    auto state = utils::Contains(selectedIndices, i) ? Qt::Checked : Qt::Unchecked;
    item->setData(state, Qt::CheckStateRole);
  }

  SetConnected(true);
  UpdateBoxLabel();
}

void SelectableComboBoxEditor::SetConnected(bool isConnected)
{
  if (isConnected)
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

//! Update text on QComboBox with the label provided by combo property.

void SelectableComboBoxEditor::UpdateBoxLabel()
{
  auto combo = GetData().value<ComboProperty>();
  m_box->setCurrentText(QString::fromStdString(combo.GetLabel()));
}

bool SelectableComboBoxEditor::IsClickToSelect(QObject* obj, QEvent* event) const
{
  return obj == m_box->view()->viewport() && event->type() == QEvent::MouseButtonRelease;
}

bool SelectableComboBoxEditor::IsClickToExpand(QObject* obj, QEvent* event) const
{
  return obj == m_box->lineEdit() && event->type() == QEvent::MouseButtonRelease;
}

}  // namespace mvvm
