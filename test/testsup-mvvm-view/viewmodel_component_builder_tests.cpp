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

#include "mvvm/views/viewmodel_component_builder.h"

#include <mvvm/editors/default_editor_factory.h>
#include <mvvm/providers/i_cell_decorator.h>
#include <mvvm/providers/i_editor_factory.h>
#include <mvvm/views/default_cell_decorator.h>
#include <mvvm/views/viewmodel_delegate.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <testutils/mock_cell_decorator.h>
#include <testutils/mock_editor_factory.h>

#include <QWidget>

using namespace mvvm;

/**
 * @brief Tests of builders in viewmodel_component_builder.h header.
 */
class ViewmodelComponentBuilderTest : public ::testing::Test
{
public:
  test::MockEditorFactory m_mock_editor_factory;
};

TEST_F(ViewmodelComponentBuilderTest, ViewModelDelegateBuilder)
{
  // here we have implicit call of "operator std::unique_ptr<ViewModelDelegate>()"
  const std::unique_ptr<ViewModelDelegate> delegate =
      ViewModelDelegateBuilder().Factory<DefaultEditorFactory>().Decorator<DefaultCellDecorator>();

  ASSERT_NE(delegate.get(), nullptr);
}

TEST_F(ViewmodelComponentBuilderTest, ViewModelDelegateBuilderWithTestFactory)
{
  const std::unique_ptr<ViewModelDelegate> delegate =
      ViewModelDelegateBuilder().Factory<test::EditorFactoryDecorator>(&m_mock_editor_factory);

  const QModelIndex index;
  EXPECT_CALL(m_mock_editor_factory, CreateEditor(index)).Times(1);

  auto editor = delegate->createEditor(nullptr, QStyleOptionViewItem(), index);
}
