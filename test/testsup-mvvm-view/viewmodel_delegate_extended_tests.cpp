/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "mvvm/views/viewmodel_delegate.h"

#include <mvvm/editors/allint_spinbox.h>
#include <mvvm/editors/scientific_spinbox.h>
#include <mvvm/model/application_model.h>
#include <mvvm/model/compound_item.h>
#include <mvvm/model/item_limits_helper.h>
#include <mvvm/model/property_item.h>
#include <mvvm/utils/i_limited_float.h>
#include <mvvm/utils/i_limited_integer.h>
#include <mvvm/viewmodel/all_items_viewmodel.h>
#include <mvvm/viewmodel/filter_name_viewmodel.h>

#include <gtest/gtest.h>

#include <QApplication>
#include <QFocusEvent>
#include <QSpinBox>
#include <QTest>
#include <QTreeView>

namespace mvvm::test
{

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

    auto& prop3 = parent0->AddProperty("N", mvvm::uint32{10});
    prop3.SetFlag(Appearance::kNullable, true);
    prop3.SetData(mvvm::uint32{42}, DataRole::kDefaultValue);
  }

  /**
   * @brief Creates editor for given model index.
   *
   * It mimicks how QTreeView creates a cell editor, when user double-click on some value.
   */
  std::unique_ptr<QWidget> CreateEditor(const QModelIndex& index) const
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

  auto parent0_index = view_model.index(0, 0, QModelIndex());
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

  {  // prop1 with AllIntSpinBox as an editor
    auto editor = CreateEditor(prop1_index);
    auto int_editor = dynamic_cast<AllIntSpinBox*>(editor.get());
    ASSERT_NE(int_editor, nullptr);
    const auto expected_min = std::numeric_limits<mvvm::int8>::lowest();
    const auto expected_max = std::numeric_limits<mvvm::int8>::max();
    EXPECT_EQ(int_editor->GetLimitedInteger()->GetLowerBoundAsVariant(),
              mvvm::variant_t(expected_min));
    EXPECT_EQ(int_editor->GetLimitedInteger()->GetUpperBoundAsVariant(),
              mvvm::variant_t(expected_max));
  }

  {  // prop2 with ScientificSpinBox as an editor
    auto editor = CreateEditor(prop2_index);
    auto float_editor = dynamic_cast<ScientificSpinBox*>(editor.get());
    ASSERT_NE(float_editor, nullptr);
    EXPECT_EQ(float_editor->GetLimitedFloat()->GetLowerBoundAsVariant(),
              variant_t(mvvm::float32{40.0}));
    EXPECT_EQ(float_editor->GetLimitedFloat()->GetUpperBoundAsVariant(),
              variant_t(mvvm::float32{50.0}));
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

  {  // prop2 with ScientificSpinBox as an editor
    auto editor = CreateEditor(prop2_index);
    auto float_editor = dynamic_cast<ScientificSpinBox*>(editor.get());
    ASSERT_NE(float_editor, nullptr);
    EXPECT_EQ(float_editor->GetLimitedFloat()->GetLowerBoundAsVariant(),
              variant_t(mvvm::float32{40.0}));
    EXPECT_EQ(float_editor->GetLimitedFloat()->GetUpperBoundAsVariant(),
              variant_t(mvvm::float32{50.0}));
  }
}

TEST_F(ViewModelDelegateExtendedTest, AllIntSpinBoxCommitOnFocusOut)
{
  AllItemsViewModel view_model(&m_model);
  QTreeView view;
  view.setModel(&view_model);
  view.setItemDelegate(&m_delegate);
  view.expandAll();

  auto parent0_index = view_model.index(0, 0, QModelIndex());
  auto prop1_index = view_model.index(1, 1, parent0_index);  // index of int8

  view.edit(prop1_index);
  auto editor = dynamic_cast<AllIntSpinBox*>(view.indexWidget(prop1_index));
  ASSERT_NE(editor, nullptr);

  // the whole text is selected, typing replaces it
  QTest::keyClicks(editor, "25");
  EXPECT_EQ(editor->text(), QString("25"));

  // focus-out without Enter commits typed value
  QFocusEvent focus_out(QEvent::FocusOut);
  QApplication::sendEvent(editor, &focus_out);

  EXPECT_EQ(view_model.GetSessionItemFromIndex(prop1_index)->Data(), variant_t(mvvm::int8{25}));
}

TEST_F(ViewModelDelegateExtendedTest, ScientificSpinBoxCommitOnFocusOut)
{
  ApplicationModel model;
  auto parent = model.InsertItem<CompoundItem>();
  auto& property = parent->AddProperty("A", 42.0);
  property.SetFlag(Appearance::kScientific, true);

  AllItemsViewModel view_model(&model);
  QTreeView view;
  view.setModel(&view_model);
  view.setItemDelegate(&m_delegate);
  view.expandAll();

  auto parent_index = view_model.index(0, 0, QModelIndex());
  auto property_index = view_model.index(0, 1, parent_index);

  view.edit(property_index);
  auto editor = dynamic_cast<ScientificSpinBox*>(view.indexWidget(property_index));
  ASSERT_NE(editor, nullptr);
  EXPECT_EQ(editor->text(), QString("4.2e+01"));

  // the whole text is selected, typing replaces it
  QTest::keyClicks(editor, "4.4647e-34");

  // focus-out without Enter commits typed value
  QFocusEvent focus_out(QEvent::FocusOut);
  QApplication::sendEvent(editor, &focus_out);

  EXPECT_EQ(property.Data(), variant_t(4.4647e-34));
}

TEST_F(ViewModelDelegateExtendedTest, NullableAllIntSpinBox)
{
  AllItemsViewModel view_model(&m_model);

  auto parent0_index = view_model.index(0, 0, QModelIndex());
  auto prop3_index = view_model.index(3, 1, parent0_index);  // index of nullable uint32

  {  // editor with value
    auto editor = CreateEditor(prop3_index);
    auto int_editor = dynamic_cast<AllIntSpinBox*>(editor.get());
    ASSERT_NE(int_editor, nullptr);
    EXPECT_TRUE(int_editor->GetLimitedInteger()->IsNullable());
    EXPECT_EQ(int_editor->text(), QString("10"));
  }

  view_model.GetSessionItemFromIndex(prop3_index)->SetData(variant_t{});

  {  // editor without value
    auto editor = CreateEditor(prop3_index);
    auto int_editor = dynamic_cast<AllIntSpinBox*>(editor.get());
    ASSERT_NE(int_editor, nullptr);
    EXPECT_EQ(int_editor->text(), QString());
    EXPECT_FALSE(int_editor->value().isValid());
    EXPECT_EQ(int_editor->GetLimitedInteger()->GetValueAsVariant(), variant_t{});
  }
}

TEST_F(ViewModelDelegateExtendedTest, SetEditorDataWithoutData)
{
  AllItemsViewModel view_model(&m_model);

  auto parent0_index = view_model.index(0, 0, QModelIndex());
  auto prop3_index = view_model.index(3, 1, parent0_index);  // index of nullable uint32

  auto editor = CreateEditor(prop3_index);
  auto int_editor = dynamic_cast<AllIntSpinBox*>(editor.get());
  ASSERT_NE(int_editor, nullptr);
  EXPECT_EQ(int_editor->text(), QString("10"));

  // data removed while the editor is open
  view_model.GetSessionItemFromIndex(prop3_index)->SetData(variant_t{});
  m_delegate.setEditorData(editor.get(), prop3_index);

  EXPECT_EQ(int_editor->text(), QString());
  EXPECT_FALSE(int_editor->value().isValid());
}

TEST_F(ViewModelDelegateExtendedTest, NullableClearOnFocusOut)
{
  AllItemsViewModel view_model(&m_model);
  QTreeView view;
  view.setModel(&view_model);
  view.setItemDelegate(&m_delegate);
  view.expandAll();

  auto parent0_index = view_model.index(0, 0, QModelIndex());
  auto prop3_index = view_model.index(3, 1, parent0_index);  // index of nullable uint32

  view.edit(prop3_index);
  auto editor = dynamic_cast<AllIntSpinBox*>(view.indexWidget(prop3_index));
  ASSERT_NE(editor, nullptr);

  // the whole text is selected, delete clears it
  QTest::keyClick(editor, Qt::Key_Delete);
  EXPECT_EQ(editor->text(), QString());

  QFocusEvent focus_out(QEvent::FocusOut);
  QApplication::sendEvent(editor, &focus_out);

  EXPECT_FALSE(view_model.GetSessionItemFromIndex(prop3_index)->HasData(DataRole::kData));
}

TEST_F(ViewModelDelegateExtendedTest, NullableStepFromNoValue)
{
  AllItemsViewModel view_model(&m_model);
  QTreeView view;
  view.setModel(&view_model);
  view.setItemDelegate(&m_delegate);
  view.expandAll();

  auto parent0_index = view_model.index(0, 0, QModelIndex());
  auto prop3_index = view_model.index(3, 1, parent0_index);  // index of nullable uint32
  view_model.GetSessionItemFromIndex(prop3_index)->SetData(variant_t{});

  view.edit(prop3_index);
  auto editor = dynamic_cast<AllIntSpinBox*>(view.indexWidget(prop3_index));
  ASSERT_NE(editor, nullptr);
  EXPECT_EQ(editor->text(), QString());

  // the first step sets the default value
  QTest::keyClick(editor, Qt::Key_Up);
  EXPECT_EQ(editor->text(), QString("42"));

  QFocusEvent focus_out(QEvent::FocusOut);
  QApplication::sendEvent(editor, &focus_out);

  EXPECT_EQ(view_model.GetSessionItemFromIndex(prop3_index)->Data(), variant_t{uint32{42}});
}

}  // namespace mvvm::test
