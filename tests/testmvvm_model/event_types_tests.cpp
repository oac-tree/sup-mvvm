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

#include "mvvm/model/sessionitem.h"
#include "mvvm/signals/event_types.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>
#include <variant>
#include <iostream>

using namespace mvvm;
using ::testing::_;

class EventTypesTests : public ::testing::Test
{
public:
  struct TestEvent
  {
    int m_data_role{0};
    SessionItem* m_item{nullptr};
    TestEvent(int value, SessionItem* item) : m_data_role(value), m_item(item)
    {
      std::cout << "AAAA " << std::endl;
    }
  };

  template <typename EventT, typename... Args>
  void Process(Args&&... args)
  {
    EventT event{std::forward<Args>(args)...};
  }
};

TEST_F(EventTypesTests, DataChangedEventCtor)
{
  TestEvent event{0, nullptr};

  SessionItem* item{nullptr};
  int value{42};

  Process<TestEvent>(value, item);
}
