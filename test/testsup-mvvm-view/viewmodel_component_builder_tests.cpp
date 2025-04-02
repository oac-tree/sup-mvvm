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
#include <mvvm/model/application_model.h>
#include <mvvm/model/property_item.h>
#include <mvvm/providers/i_cell_decorator.h>
#include <mvvm/providers/i_editor_factory.h>
#include <mvvm/viewmodel/all_items_viewmodel.h>
#include <mvvm/views/default_cell_decorator.h>
#include <mvvm/views/item_view_component_provider.h>
#include <mvvm/views/viewmodel_delegate.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <testutils/mock_cell_decorator.h>
#include <testutils/mock_editor_factory.h>

#include <QTreeView>
#include <QWidget>

using namespace mvvm;

/**
 * @brief Tests of builders in viewmodel_component_builder.h header.
 */
class ViewmodelComponentBuilderTest : public ::testing::Test
{
public:
  test::MockEditorFactory m_mock_editor_factory;
  mvvm::ApplicationModel m_model;
  QTreeView m_view;
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

TEST_F(ViewmodelComponentBuilderTest, ItemViewComponentProviderBuilder)
{
  auto item = m_model.InsertItem<PropertyItem>();
  item->SetData(42);

  std::unique_ptr<mvvm::ItemViewComponentProvider> provider =
      mvvm::ItemViewComponentProviderBuilder()
          .ViewModel<AllItemsViewModel>(&m_model)
          .View(&m_view)
          .Factory<DefaultEditorFactory>()
          .Decorator<DefaultCellDecorator>();

  EXPECT_EQ(provider->GetView(), &m_view);
  EXPECT_NE(provider->GetViewModel(), nullptr);
  EXPECT_EQ(provider->GetViewModel()->GetModel(), &m_model);

  auto delegate = dynamic_cast<ViewModelDelegate*>(m_view.itemDelegate());
  ASSERT_NE(delegate, nullptr);
}

TEST_F(ViewmodelComponentBuilderTest, CreateProvider)
{
  auto item = m_model.InsertItem<PropertyItem>();
  item->SetData(42);

  std::unique_ptr<ItemViewComponentProvider> provider = CreateProvider()
                                                            .ViewModel<AllItemsViewModel>(&m_model)
                                                            .View(&m_view)
                                                            .Factory<DefaultEditorFactory>()
                                                            .Decorator<DefaultCellDecorator>();

  EXPECT_EQ(provider->GetView(), &m_view);
  EXPECT_NE(provider->GetViewModel(), nullptr);
  EXPECT_EQ(provider->GetViewModel()->GetModel(), &m_model);
}

TEST_F(ViewmodelComponentBuilderTest, CreateProviderWithDefaultDelegate)
{
  const std::unique_ptr<ItemViewComponentProvider> provider(
      CreateProvider().ViewModel<AllItemsViewModel>(&m_model).View(&m_view));

  auto delegate = dynamic_cast<ViewModelDelegate*>(m_view.itemDelegate());
  ASSERT_NE(delegate, nullptr);
}

TEST_F(ViewmodelComponentBuilderTest, CreateProviderWithDefaultDelegateWHenNoModelIsKnown)
{
  const std::unique_ptr<ItemViewComponentProvider> provider(
      CreateProvider().ViewModel<AllItemsViewModel>().View(&m_view));

  auto delegate = dynamic_cast<ViewModelDelegate*>(m_view.itemDelegate());
  ASSERT_NE(delegate, nullptr);
  EXPECT_EQ(provider->GetViewModel()->GetModel(), nullptr);
}
