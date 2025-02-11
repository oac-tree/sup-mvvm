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

#include "mvvm/providers/viewmodel_delegate.h"

#include <mvvm/editors/custom_editor_includes.h>
#include <mvvm/model/application_model.h>
#include <mvvm/model/compound_item.h>
#include <mvvm/model/item_limits_helper.h>
#include <mvvm/model/property_item.h>
#include <mvvm/standarditems/editor_constants.h>
#include <mvvm/viewmodel/all_items_viewmodel.h>
#include <mvvm/viewmodel/filter_name_viewmodel.h>

#include <gtest/gtest.h>

#include <QSpinBox>

using namespace mvvm;

/**
 * @brief Tests for editor factories involving a complex model with filtering capabilities.
 */
class ViewModelDelegateExtendedTest : public ::testing::Test
{
public:
  ViewModelDelegateExtendedTest()
  {
    auto parent0 = m_model.InsertItem<CompoundItem>();
    parent0->SetDisplayName("parent0");

    auto& prop0 = parent0->AddProperty("A", 42);
    mvvm::SetLimited(30, 50, prop0);

    auto& prop1 = parent0->AddProperty("AB", mvvm::int8{20});
    (void)prop1;

    auto& prop2 = parent0->AddProperty("ABC", mvvm::float32{42.0});
    mvvm::SetLimited(mvvm::float32{40}, mvvm::float32{50}, prop2);
  }

  /**
   * @brief Creates editor for given model index.
   *
   * It mimicks how QTreeView creates a cell editor, when user double-click on some value.
   */
  std::unique_ptr<QWidget> CreateEditor(const QModelIndex& index)
  {
    auto result =
        std::unique_ptr<QWidget>(m_delegate.createEditor(nullptr, QStyleOptionViewItem(), index));
    // to get proper values and limits set inside the editor
    m_delegate.setEditorData(result.get(), index);
    return result;
  }

  ApplicationModel m_model;
  ViewModelDelegate m_delegate;
};

TEST_F(ViewModelDelegateExtendedTest, ViewModelWithoutFiltering)
{
  const AllItemsViewModel view_model(&m_model);

  auto parent0_index = view_model.index(0, 0);
  auto prop0_index = view_model.index(0, 1, parent0_index);  // index of int
  auto prop1_index = view_model.index(1, 1, parent0_index);  // index of int8
  auto prop2_index = view_model.index(2, 1, parent0_index);  // index of float32

  {  // prop0 with QSpinBox as an editor
    auto editor = CreateEditor(prop0_index);
    auto int_editor = dynamic_cast<QSpinBox*>(editor.get());
    ASSERT_NE(int_editor, nullptr);
    EXPECT_EQ(int_editor->minimum(), 30);
    EXPECT_EQ(int_editor->maximum(), 50);
  }

  {  // prop1 with AllIntSpinBoxEditor as an editor
    auto editor = CreateEditor(prop1_index);
    auto int_editor = dynamic_cast<AllIntSpinBoxEditor*>(editor.get());
    ASSERT_NE(int_editor, nullptr);
    const auto expected_min = std::numeric_limits<mvvm::int8>::lowest();
    const auto expected_max = std::numeric_limits<mvvm::int8>::max();
    EXPECT_EQ(int_editor->GetLowerLimit(), mvvm::variant_t(expected_min));
    EXPECT_EQ(int_editor->GetUpperLimit(), mvvm::variant_t(expected_max));
  }

  {  // prop1 with FloatSpinBox as an editor
    auto editor = CreateEditor(prop2_index);
    auto float_editor = dynamic_cast<FloatSpinBox*>(editor.get());
    ASSERT_NE(float_editor, nullptr);
    EXPECT_EQ(float_editor->minimum(), 40.0);
    EXPECT_EQ(float_editor->maximum(), 50.0);
  }
}

TEST_F(ViewModelDelegateExtendedTest, ViewModelWithFiltering)
{
  AllItemsViewModel view_model(&m_model);

  FilterNameViewModel filtered_model;
  filtered_model.setSourceModel(&view_model);
  filtered_model.SetPattern("ABC");

  // looking to editors via filtered model
  auto parent0_index = filtered_model.index(0, 0);
  // two property items have been filtered out, one property remains
  ASSERT_EQ(filtered_model.rowCount(parent0_index), 1);
  auto prop2_index = filtered_model.index(0, 1, parent0_index);  // index of float32

  {  // prop1 with FloatSpinBox as an editor
    auto editor = CreateEditor(prop2_index);
    auto float_editor = dynamic_cast<FloatSpinBox*>(editor.get());
    ASSERT_NE(float_editor, nullptr);
    EXPECT_EQ(float_editor->minimum(), 40.0);
    EXPECT_EQ(float_editor->maximum(), 50.0);
  }
}
