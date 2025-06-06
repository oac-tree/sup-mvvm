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

#include "mvvm/views/property_grid_controller.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/editors/float_spinbox.h>
#include <mvvm/model/application_model.h>
#include <mvvm/model/property_item.h>
#include <mvvm/standarditems/vector_item.h>
#include <mvvm/viewmodel/all_items_viewmodel.h>
#include <mvvm/viewmodel/property_table_viewmodel.h>
#include <mvvm/viewmodel/property_viewmodel.h>
#include <mvvm/views/viewmodel_delegate.h>

#include <gtest/gtest.h>

#include <QDataWidgetMapper>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QLineEdit>
#include <QSignalSpy>
#include <QSpinBox>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QTest>
#include <QWidget>

using namespace mvvm;

/**
 * @brief Testing PropertyGridController.
 */
class PropertyGridControllerTest : public ::testing::Test
{
public:
  static QList<QStandardItem*> CreateItemRow(const QString& name, int value)
  {
    QList<QStandardItem*> result = {new QStandardItem("a"), new QStandardItem("b")};
    result.at(0)->setData(name, Qt::EditRole);
    result.at(1)->setData(value, Qt::EditRole);
    return result;
  }
};

//! To learn how QDataWidgetMapper is functioning.
TEST_F(PropertyGridControllerTest, DataWidgetMapperBasics)
{
  QStandardItemModel view_model;
  auto parent_item = view_model.invisibleRootItem();
  parent_item->insertRow(0, CreateItemRow("abc", 42));
  parent_item->insertRow(1, CreateItemRow("def", 43));

  // setting up external widgets and the mapper
  QLineEdit line_edit;
  QSpinBox spin_box;
  QDataWidgetMapper widget_mapper;
  widget_mapper.setModel(&view_model);
  widget_mapper.addMapping(&line_edit, 0);
  widget_mapper.addMapping(&spin_box, 1);
  widget_mapper.toFirst();

  // checking values in the model
  EXPECT_EQ(view_model.data(view_model.index(0, 0)).toString(), QString("abc"));
  EXPECT_EQ(view_model.data(view_model.index(0, 1)).toInt(), 42);

  // checking values via editor
  EXPECT_EQ(line_edit.text(), QString("abc"));
  EXPECT_EQ(spin_box.value(), 42);

  // modifying cell via external editor
  spin_box.setValue(98);

  widget_mapper.submit();

  // checking new values through widget, and directly in the model
  EXPECT_EQ(spin_box.value(), 98);
  EXPECT_EQ(view_model.data(view_model.index(0, 1)).toInt(), 98);
}

//! To learn how QDataWidgetMapper is functioning when mapping QLabel.
TEST_F(PropertyGridControllerTest, DataWidgetMapperAndQLabel)
{
  QStandardItemModel view_model;
  auto parent_item = view_model.invisibleRootItem();
  parent_item->insertRow(0, CreateItemRow("abc", 42));

  // setting up external widgets and the mapper
  QLabel label;
  QDataWidgetMapper widget_mapper;
  widget_mapper.setModel(&view_model);
  // non-editable widgets needs third "property" parameter for mapping
  // See QTBUG-10672
  widget_mapper.addMapping(&label, 0, "text");
  widget_mapper.toFirst();

  // checking values in the model and widget
  EXPECT_EQ(view_model.data(view_model.index(0, 0)).toString(), QString("abc"));
  EXPECT_EQ(label.text(), QString("abc"));

  // changing the value via model
  view_model.setData(view_model.index(0, 0), QString("def"), Qt::EditRole);
  EXPECT_EQ(label.text(), QString("def"));

  // changing color, text should be the same (this wasn't the case in PropertyFlatView)
  view_model.setData(view_model.index(0, 0), QColor(Qt::red), Qt::ForegroundRole);
  EXPECT_EQ(label.text(), QString("def"));
}

//! To learn how QDataWidgetMapper is functioning when mapping QLabel.
TEST_F(PropertyGridControllerTest, DataWidgetMapperAndQLabelWithDelegatePresence)
{
  QStandardItemModel view_model;
  auto parent_item = view_model.invisibleRootItem();
  parent_item->insertRow(0, CreateItemRow("abc", 42));

  // setting up external widgets and the mapper
  QSpinBox spinbox;
  QLabel label;
  QDataWidgetMapper widget_mapper;
  ViewModelDelegate delegate;
  widget_mapper.setModel(&view_model);
  widget_mapper.setItemDelegate(&delegate);
  // non-editable widgets needs third "property" parameter for mapping
  // See QTBUG-10672
  widget_mapper.addMapping(&label, 0, "text");
  widget_mapper.addMapping(&spinbox, 1);
  widget_mapper.setCurrentIndex(0);

  // checking values in the model and widget
  EXPECT_EQ(view_model.data(view_model.index(0, 0)).toString(), QString("abc"));
  EXPECT_EQ(label.text(), QString("abc"));
  EXPECT_EQ(spinbox.value(), 42);

  // changing the value via model
  view_model.setData(view_model.index(0, 0), QString("def"), Qt::EditRole);
  EXPECT_EQ(label.text(), QString("def"));

  // changing color, text should be the same (this wasn't the case in PropertyFlatView)
  view_model.setData(view_model.index(0, 0), QColor(Qt::red), Qt::ForegroundRole);
  EXPECT_EQ(label.text(), QString("def"));
}

TEST_F(PropertyGridControllerTest, DataWidgetMapperAndQLabelWithOurViewModel)
{
  ApplicationModel model;
  auto item = model.InsertItem<PropertyItem>();
  item->SetDisplayName("abc");
  item->SetData(42);

  AllItemsViewModel view_model(&model);

  QSpinBox spinbox;
  QLabel label;
  QDataWidgetMapper widget_mapper;
  ViewModelDelegate delegate;
  widget_mapper.setModel(&view_model);
  widget_mapper.setItemDelegate(&delegate);
  // non-editable widgets needs third "property" parameter for mapping
  // See QTBUG-10672
  widget_mapper.addMapping(&label, 0, "text");
  widget_mapper.addMapping(&spinbox, 1);
  widget_mapper.setCurrentIndex(0);

  EXPECT_EQ(view_model.data(view_model.index(0, 0)), QString("abc"));
  EXPECT_EQ(label.text(), QString("abc"));

  item->SetEnabled(false);

  EXPECT_EQ(label.text(), QString("abc"));
  EXPECT_EQ(item->GetDisplayName(), "abc");
}

TEST_F(PropertyGridControllerTest, UninitialisedModel)
{
  EXPECT_THROW(PropertyGridController(nullptr), NullArgumentException);
}

//! Checking method CreateWidget. Use QStandardItemModel with single row with label and data.
TEST_F(PropertyGridControllerTest, CreateWidget)
{
  // preparing the model
  QStandardItemModel view_model;
  auto parent_item = view_model.invisibleRootItem();
  const QList<QStandardItem*> items = {new QStandardItem("a"), new QStandardItem("b")};

  // first item in a row is a label
  items.at(0)->setEditable(false);
  items.at(0)->setData("abc", Qt::DisplayRole);
  parent_item->insertRow(0, items);
  // second item in a row carries an integer
  items.at(1)->setData(42, Qt::EditRole);

  PropertyGridController controller(&view_model);
  auto widgets = controller.CreateWidgetGrid();

  // checking that controller can create proper editors for columns
  auto label_widget = dynamic_cast<QLabel*>(widgets[0][0].get());
  auto spinbox_widget = dynamic_cast<QSpinBox*>(widgets[0][1].get());
  EXPECT_TRUE(label_widget);
  EXPECT_TRUE(spinbox_widget);

  // checking that widgets get initial values correctly
  EXPECT_EQ(label_widget->text(), QString("abc"));
  EXPECT_EQ(spinbox_widget->value(), 42);
}

//! Checking method CreateGrid. Use QStandardItemModel with two rows and three columns.
TEST_F(PropertyGridControllerTest, CreateGridForStandardModel)
{
  // preparing the model
  QStandardItemModel view_model;
  auto parent_item = view_model.invisibleRootItem();
  const QList<QStandardItem*> row0 = {new QStandardItem("a"), new QStandardItem("b"),
                                      new QStandardItem("b")};
  const QList<QStandardItem*> row1 = {new QStandardItem("a"), new QStandardItem("b"),
                                      new QStandardItem("b")};
  parent_item->insertRow(0, row0);
  parent_item->insertRow(0, row1);

  PropertyGridController controller(&view_model);

  auto editor_grid = controller.CreateWidgetGrid();

  EXPECT_EQ(editor_grid.size(), 2);
  EXPECT_EQ(editor_grid[0].size(), 3);
}

//! Checking method CreateGrid. Use PropertyViewModel and single VectorItem in it.
TEST_F(PropertyGridControllerTest, CreateGridForPropertyViewModel)
{
  ApplicationModel model;
  auto parent = model.InsertItem<VectorItem>();

  PropertyViewModel view_model(&model);
  view_model.SetRootSessionItem(parent);

  PropertyGridController controller(&view_model);
  auto editor_grid = controller.CreateWidgetGrid();

  EXPECT_EQ(editor_grid.size(), 3);
  EXPECT_EQ(editor_grid[0].size(), 2);
}

TEST_F(PropertyGridControllerTest, CreateGridForPropertyItemDisabled)
{
  ApplicationModel model;
  auto item = model.InsertItem<PropertyItem>();
  item->SetDisplayName("aaa");
  item->SetData(42);

  AllItemsViewModel view_model(&model);

  PropertyGridController controller(&view_model);
  auto editor_grid = controller.CreateWidgetGrid();

  item->SetDisplayName("abc");

  ASSERT_EQ(editor_grid.size(), 1);
  ASSERT_EQ(editor_grid[0].size(), 2);

  auto label = dynamic_cast<QLabel*>(editor_grid[0][0].get());
  ASSERT_NE(label, nullptr);
  EXPECT_EQ(view_model.data(view_model.index(0, 0)), QString("abc"));
  EXPECT_EQ(label->text(), QString("abc"));

  item->SetEnabled(false);

  ASSERT_NE(label, nullptr);
  EXPECT_EQ(label->text(), QString("abc"));
  EXPECT_EQ(item->GetDisplayName(), "abc");
}

//! Checking method CreateGrid. Use PropertyViewModel and single VectorItem in it.
//! Property item representing "y" is disabled. Real-life bug.
TEST_F(PropertyGridControllerTest, CreateGridForPropertyViewModelDisabled)
{
  ApplicationModel model;
  auto parent = model.InsertItem<VectorItem>();

  PropertyViewModel view_model(&model);
  view_model.SetRootSessionItem(parent);

  PropertyGridController controller(&view_model);
  auto editor_grid = controller.CreateWidgetGrid();

  ASSERT_EQ(editor_grid.size(), 3);
  ASSERT_EQ(editor_grid[0].size(), 2);
  ASSERT_EQ(editor_grid[1].size(), 2);
  ASSERT_EQ(editor_grid[2].size(), 2);

  auto y_label = dynamic_cast<QLabel*>(editor_grid[1][0].get());
  EXPECT_EQ(y_label->text(), QString("Y"));

  EXPECT_TRUE(y_label->isEnabled());
  EXPECT_TRUE(editor_grid[1][1]->isEnabled());

  parent->GetItem(VectorItem::kY)->SetEnabled(false);

  ASSERT_NE(y_label, nullptr);
  EXPECT_FALSE(y_label->isEnabled());
  EXPECT_FALSE(editor_grid[1][1]->isEnabled());
  EXPECT_EQ(y_label->text(), QString("Y"));
  EXPECT_EQ(parent->GetItem(VectorItem::kY)->GetDisplayName(), "Y");
}

//! Checking method CreateGrid for VectorItem in PropertyViewModel. VectorItem is set after
//! controller creation, thus checking reset signal.
TEST_F(PropertyGridControllerTest, CreateGridForVectorItemProperties)
{
  ApplicationModel model;
  auto parent = model.InsertItem<VectorItem>();

  PropertyViewModel view_model(&model);

  PropertyGridController controller(&view_model);
  const QSignalSpy spy_grid_changed(&controller, &PropertyGridController::GridChanged);

  view_model.SetRootSessionItem(parent);

  auto editor_grid = controller.CreateWidgetGrid();

  EXPECT_EQ(editor_grid.size(), 3);
  EXPECT_EQ(editor_grid[0].size(), 2);

  EXPECT_EQ(spy_grid_changed.count(), 1);
}

//! Checking method CreateGrid. Use PropertyTableViewModel with two VectorItem in it.
TEST_F(PropertyGridControllerTest, CreateGridForPropertyTableViewModel)
{
  ApplicationModel model;
  model.InsertItem<VectorItem>();
  model.InsertItem<VectorItem>();

  PropertyTableViewModel view_model(&model);

  PropertyGridController controller(&view_model);
  auto editor_grid = controller.CreateWidgetGrid();

  EXPECT_EQ(editor_grid.size(), 2);
  EXPECT_EQ(editor_grid[0].size(), 3);
}

//! Checking controller signaling while inserting item through the model.
TEST_F(PropertyGridControllerTest, GridChanged)
{
  ApplicationModel model;

  AllItemsViewModel view_model(&model);

  const PropertyGridController controller(&view_model);

  const QSignalSpy spy_grid_changed(&controller, &PropertyGridController::GridChanged);

  model.InsertItem<PropertyItem>();

  EXPECT_EQ(spy_grid_changed.count(), 1);
}

//! Validating that internal mapping is working. The data is set via the editor
TEST_F(PropertyGridControllerTest, SetDataThroughObtainedEditor)
{
  ApplicationModel model;
  auto vector = model.InsertItem<VectorItem>();

  PropertyViewModel view_model(&model);
  view_model.SetRootSessionItem(vector);

  PropertyGridController controller(&view_model);
  auto editor_grid = controller.CreateWidgetGrid();

  // we expect here a grid (row, col) = (3, 2) of widgets
  // the first column is a label, the second is an editor
  EXPECT_EQ(editor_grid.size(), 3);
  EXPECT_EQ(editor_grid[0].size(), 2);

  auto x_label = dynamic_cast<QLabel*>(editor_grid[0][0].get());
  EXPECT_NE(x_label, nullptr);
  auto x_double_spin_box = dynamic_cast<FloatSpinBox*>(editor_grid[0][1].get());
  EXPECT_NE(x_double_spin_box, nullptr);

  // setting the data
  x_double_spin_box->setValue(42.1);

  EXPECT_TRUE(controller.Submit());

  EXPECT_DOUBLE_EQ(vector->X(), 42.1);
}

//! Validating that internal mapping is working. The data is set via the model and then checked in
//! the editor
TEST_F(PropertyGridControllerTest, SetDataThroughModel)
{
  ApplicationModel model;
  auto root_item = model.InsertItem<CompoundItem>();

  auto& editable_property = root_item->AddProperty("edit", 0.0).SetDisplayName("Editable Property");
  auto& non_editable_property = root_item->AddProperty("nedit", 0.0)
                                    .SetDisplayName("Non-editable Property")
                                    .SetEditable(false);

  PropertyViewModel view_model(&model);
  view_model.SetRootSessionItem(root_item);

  PropertyGridController controller(&view_model);
  auto editor_grid = controller.CreateWidgetGrid();

  // we expect here a grid (row, col) = (2, 2) of widgets
  // the first column is a label, the second is an editor/label
  EXPECT_EQ(editor_grid.size(), 2);
  EXPECT_EQ(editor_grid[0].size(), 2);

  // First row is editable property
  // The first column is a label, the second is an editor
  EXPECT_NE(dynamic_cast<QLabel*>(editor_grid[0][0].get()), nullptr);
  auto editable_double_spin_box = dynamic_cast<FloatSpinBox*>(editor_grid[0][1].get());
  EXPECT_NE(editable_double_spin_box, nullptr);

  // First row is non-editable property
  // The first and second column are a label
  EXPECT_NE(dynamic_cast<QLabel*>(editor_grid[1][0].get()), nullptr);
  auto non_editable_value_label = dynamic_cast<QLabel*>(editor_grid[1][1].get());
  EXPECT_NE(non_editable_value_label, nullptr);

  // Original data
  EXPECT_DOUBLE_EQ(editable_double_spin_box->value().toDouble(), 0.0);
  EXPECT_DOUBLE_EQ(non_editable_value_label->text().toDouble(), 0.0);

  // Setting the data via model
  editable_property.SetData(42.1);
  non_editable_property.SetData(42.1);

  // Checking that data is set on widgets
  EXPECT_DOUBLE_EQ(editable_double_spin_box->value().toDouble(), 42.1);
  EXPECT_DOUBLE_EQ(non_editable_value_label->text().toDouble(), 42.1);
}

//! Validating that GridController survives after model clearing.
TEST_F(PropertyGridControllerTest, ClearModel)
{
  ApplicationModel model;
  auto vector = model.InsertItem<VectorItem>();

  PropertyViewModel view_model(&model);
  view_model.SetRootSessionItem(vector);

  PropertyGridController controller(&view_model);

  {
    // set of editors created and immediately deleted
    [[maybe_unused]] auto editor_grid0 = controller.CreateWidgetGrid();
  }
  // clear the model
  model.Clear();

  // we populate model again and create new set of editors
  vector = model.InsertItem<VectorItem>();
  view_model.SetRootSessionItem(vector);
  auto editor_grid = controller.CreateWidgetGrid();

  // we expect here a grid (row, col) = (3, 2) of widgets
  // the first column is a label, the second is an editor
  EXPECT_EQ(editor_grid.size(), 3);
  EXPECT_EQ(editor_grid[0].size(), 2);

  auto x_label = dynamic_cast<QLabel*>(editor_grid[0][0].get());
  EXPECT_NE(x_label, nullptr);
  auto x_double_spin_box = dynamic_cast<FloatSpinBox*>(editor_grid[0][1].get());
  EXPECT_NE(x_double_spin_box, nullptr);

  // setting the data
  x_double_spin_box->setValue(42.1);

  // should not be segfault, controller should forget about previous editors
  EXPECT_TRUE(controller.Submit());

  EXPECT_DOUBLE_EQ(vector->X(), 42.1);
}
