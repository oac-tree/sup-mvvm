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

#include "mvvm/delegates/viewmodel_delegate.h"

#include <mvvm/editors/custom_editor.h>
#include <mvvm/editors/custom_editor_includes.h>
#include <mvvm/model/application_model.h>
#include <mvvm/standarditems/vector_item.h>
#include <mvvm/viewmodel/all_items_viewmodel.h>

#include <gtest/gtest.h>

#include <QDataWidgetMapper>
#include <QStyleOptionViewItem>

using namespace mvvm;

//! Tests of ViewModelDelegate class.

class ViewModelDelegateTests : public ::testing::Test
{
public:
  ViewModelDelegateTests() : m_view_model(&m_model)
  {
    m_mapper.setModel(&m_view_model);
    m_mapper.setItemDelegate(&m_delegate);
  }

  //! Returns index pointining to ViewModel cells looking at our data
  QModelIndex GetIndex(const variant_t& data)
  {
    // creating item in a model and setting data to it
    auto item = m_model.InsertItem<PropertyItem>();
    item->SetData(data);

    // at this point ViewModel was automatically updated, column = 1 is a cell looking to our data
    auto indexes = m_view_model.GetIndexOfSessionItem(item);
    return indexes.back();  // it should be two indices, the second is looking to our data
  }

  std::unique_ptr<QWidget> CreateEditor(const QModelIndex& index)
  {
    return std::unique_ptr<QWidget>(
        m_delegate.createEditor(nullptr, QStyleOptionViewItem(), index));
  }

  void map_to_index(QWidget* widget, const QModelIndex& index)
  {
    m_mapper.setRootIndex(index.parent());
    m_mapper.setCurrentModelIndex(index.sibling(index.row(), 0));
    m_mapper.addMapping(widget, 1);
  }

  ApplicationModel m_model{};
  AllItemsViewModel m_view_model;
  ViewModelDelegate m_delegate;
  QDataWidgetMapper m_mapper;
};

TEST_F(ViewModelDelegateTests, createEditor)
{
  auto index1 = GetIndex(variant_t(true));
  EXPECT_TRUE(CreateEditor(index1).get() != nullptr);
  EXPECT_TRUE(dynamic_cast<BoolEditor*>(CreateEditor(index1).get()));

  auto index2 = GetIndex(variant_t(42));
  EXPECT_TRUE(CreateEditor(index2).get() != nullptr);
}

//! Check that ViewModelDelegate can work with widget mapper.

//TEST_F(ViewModelDelegateTests, widgetMapper)
//{
//  TestData test_data;
//  auto vector_item = test_data.model.insertItem<VectorItem>();
//  auto x_item = vector_item->getItem(VectorItem::P_X);

//  // accessing to index list (index of label field and index of data field)
//  // of PropertyItem corresponding to x-coordinate.
//  auto x_value_index = test_data.view_model.indexOfSessionItem(x_item).at(1);
//  auto editor = test_data.create_editor(x_value_index);

//  test_data.map_to_index(editor.get(), x_value_index);

//  editor->setData(43.0);
//  editor->dataChanged(editor->data());
//  EXPECT_EQ(x_item->data<double>(), 43.0);
//}
