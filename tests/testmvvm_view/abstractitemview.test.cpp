/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/widgets/abstractitemview.h"

#include "mvvm/model/applicationmodel.h"
#include "mvvm/model/compounditem.h"
#include "mvvm/viewmodel/topitemsviewmodel.h"
#include "mvvm/widgets/itemselectionmodel.h"

#include <gtest/gtest.h>

#include <QSignalSpy>
#include <QTreeView>

using namespace mvvm;

class AbstractItemViewTest : public ::testing::Test
{
public:
  class TestView : public mvvm::AbstractItemView
  {
  public:
    explicit TestView(mvvm::ApplicationModel* model) : AbstractItemView(model)
    {
      SetView(new QTreeView);
      SetApplicationModel(model);
    }

  private:
    std::unique_ptr<mvvm::ViewModel> CreateViewModel(mvvm::ApplicationModel* model) override
    {
      return std::make_unique<mvvm::TopItemsViewModel>(model);
    }
  };

  ApplicationModel m_model;
};

TEST_F(AbstractItemViewTest, InitialState)
{
  TestView view(nullptr);
  EXPECT_EQ(view.GetViewModel(), nullptr);
  EXPECT_EQ(view.GetSelectedItem(), nullptr);
  EXPECT_TRUE(view.GetSelectedItems().empty());
}

TEST_F(AbstractItemViewTest, ModelInConstructor)
{
  TestView view(&m_model);
  EXPECT_NE(view.GetViewModel(), nullptr);
  EXPECT_EQ(view.GetSelectedItem(), nullptr);

  EXPECT_EQ(view.GetViewModel()->GetRootSessionItem(), m_model.GetRootItem());
}

TEST_F(AbstractItemViewTest, SetApplicationModel)
{
  TestView view(nullptr);

  view.SetApplicationModel(&m_model);
  EXPECT_NE(view.GetViewModel(), nullptr);
  EXPECT_EQ(view.GetSelectedItem(), nullptr);
  EXPECT_EQ(view.GetViewModel()->GetRootSessionItem(), m_model.GetRootItem());
}
