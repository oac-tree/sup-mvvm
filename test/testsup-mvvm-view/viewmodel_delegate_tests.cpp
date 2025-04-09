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

#include "mvvm/views/viewmodel_delegate.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/editors/custom_editor_includes.h>
#include <mvvm/editors/default_editor_factory.h>
#include <mvvm/model/application_model.h>
#include <mvvm/providers/custom_editor.h>
#include <mvvm/standarditems/vector_item.h>
#include <mvvm/viewmodel/all_items_viewmodel.h>
#include <mvvm/views/default_cell_decorator.h>

#include <gtest/gtest.h>

#include <QStyleOptionViewItem>

using namespace mvvm;

/**
 * @brief Tests for ViewModelDelegate class.
 */
class ViewModelDelegateTest : public ::testing::Test
{
public:
  ViewModelDelegateTest() : m_view_model(&m_model) {}

  /**
   * @brief Creates an entry in viewmodel containing given data.
   *
   * @param data The data to put in viewmodel
   * @return The index of viewmodel cell.
   */
  QModelIndex CreateViewModelEntry(const variant_t& data)
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

  ApplicationModel m_model;
  AllItemsViewModel m_view_model;
  ViewModelDelegate m_delegate;
};

TEST_F(ViewModelDelegateTest, InitialState)
{
  EXPECT_THROW(ViewModelDelegate({}, {}), RuntimeException);
  EXPECT_THROW(ViewModelDelegate(std::make_unique<DefaultEditorFactory>(), {}), RuntimeException);
  EXPECT_NO_THROW(ViewModelDelegate(std::make_unique<DefaultEditorFactory>(),
                                    std::make_unique<DefaultCellDecorator>()));
}

TEST_F(ViewModelDelegateTest, createEditor)
{
  auto index1 = CreateViewModelEntry(variant_t(true));
  EXPECT_TRUE(CreateEditor(index1).get() != nullptr);
  EXPECT_TRUE(dynamic_cast<BoolEditor*>(CreateEditor(index1).get()));

  auto index2 = CreateViewModelEntry(variant_t(42));
  EXPECT_TRUE(CreateEditor(index2).get() != nullptr);
}
