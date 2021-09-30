/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/viewmodel/viewmodel.h"

#include "mvvm/viewmodel/abstractviewmodelcontroller.h"
#include "mvvm/viewmodel/viewmodelcontroller.h"

#include <gtest/gtest.h>

#include <memory>

using namespace ModelView;

//! Tests ViewModel class.

class ViewModelTest : public ::testing::Test
{
public:
  class TestController : public AbstractViewModelController
  {
  public:
    TestController(SessionModel *model, ViewModelBase *view_model) {}
  };

  class TestModel : public ViewModel
  {
  public:
    TestModel(SessionModel *model)
    {
      auto controller = std::make_unique<TestController>(model, this);
      SetController(std::move(controller));
    }
  };
};

//! The map is initially empty.

TEST_F(ViewModelTest, InitialState)
{
  ViewModel view_model;
  EXPECT_EQ(view_model.rowCount(), 0);
  EXPECT_EQ(view_model.columnCount(), 0);
  EXPECT_THROW(view_model.GetSessionItemFromIndex(QModelIndex()), std::runtime_error);
}
