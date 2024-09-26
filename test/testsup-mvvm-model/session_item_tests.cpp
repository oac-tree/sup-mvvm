/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/model/application_model.h"

#include <mvvm/model/compound_item.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace mvvm;
using ::testing::_;

/**
 * @brief Tests for SessionItem.
 */
class SessionItemTest : public ::testing::Test
{
public:
  /**
   * @brief The TestItem class is a test helper that allows to test item activation.
   */
  class TestItem : public CompoundItem
  {
  public:
    TestItem(const std::function<void(ISessionModel*)>& callback)
        : CompoundItem("TestItem"), m_activate_callback(callback)
    {
      RegisterTag(TagInfo::CreateUniversalTag("tag"), true);
      AddProperty("value", 42);
    }

    void Activate() override
    {
      // We want to know how process of activation went. Children should know already their model.
      // This will allow to subscribe to their signals on parent's activation.
      m_activate_callback(GetItem("value")->GetModel());
    }

    std::function<void(ISessionModel*)> m_activate_callback;
  };
};

//! Validating item activation. We want to make sure that all children knows their model already, at
//! the moment of parent activation.
TEST_F(SessionItemTest, Activate)
{
  ::testing::MockFunction<std::function<void(ISessionModel*)>> mock_listener;

  ApplicationModel model;

  EXPECT_CALL(mock_listener, Call(&model)).Times(1);
  auto item = std::make_unique<TestItem>(mock_listener.AsStdFunction());
  model.InsertItem(std::move(item), model.GetRootItem(), TagIndex::Append());
}
