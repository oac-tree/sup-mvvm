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

#include "mvvm/viewmodel/abstractviewmodelcontroller.h"

#include "mockmodelnotifier.h"
#include "test_utils.h"

#include "mvvm/model/sessionitem.h"
#include "mvvm/viewmodel/modeleventnotifier.h"

#include <gtest/gtest.h>

using namespace ModelView;

//! Tests for AbstractViewModelController class.

class AbstractViewModelControllerTest : public ::testing::Test
{
public:
  class TestController : public AbstractViewModelController
  {
  public:
    MOCK_METHOD2(OnDataChanged, void(ModelView::SessionItem* item, int role));

    ModelEventNotifierInterface* GetNotifier() { return m_notifier; }
  };
};

//! Controller subscription.

TEST_F(AbstractViewModelControllerTest, SubscribeTo)
{
  ModelView::SessionItem item;
  int role{42};

  ModelView::ModelEventNotifier notifier;

  auto controller = std::make_unique<TestController>();

  notifier.Subscribe(controller.get());
  EXPECT_EQ(controller->GetNotifier(), &notifier);

  EXPECT_CALL(*controller, OnDataChanged(&item, role)).Times(1);

  notifier.DataChangedNotify(&item, role);
}

//! Controller unsubscription on deletion.

TEST_F(AbstractViewModelControllerTest, Unsubscribe)
{
  ModelView::SessionItem item;
  int role{42};

  ModelView::ModelEventNotifier notifier;

  auto controller = std::make_unique<TestController>();

  notifier.Subscribe(controller.get());
  EXPECT_EQ(controller->GetNotifier(), &notifier);

  controller.reset();

  ASSERT_NO_FATAL_FAILURE(notifier.DataChangedNotify(&item, role));
}

//! Check the case when EventNotifier is destroyed before the listener

TEST_F(AbstractViewModelControllerTest, DestroyNotifierBefore)
{
  ModelView::SessionItem item;
  int role{42};

  auto notifier = std::make_unique<ModelView::ModelEventNotifier>();

  auto controller = std::make_unique<TestController>();

  notifier->Subscribe(controller.get());

  // destroying notifier
  notifier.reset();

  ASSERT_NO_FATAL_FAILURE(controller.reset());
}
