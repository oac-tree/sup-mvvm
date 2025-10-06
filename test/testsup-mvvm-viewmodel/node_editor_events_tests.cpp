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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/nodeeditor/node_editor_events.h"

#include <mvvm/nodeeditor/node_port.h>

#include <gtest/gtest.h>

namespace mvvm::test
{

class NodeEditorEventsTest : public ::testing::Test
{
};

TEST_F(NodeEditorEventsTest, DoubleClickEvent)
{
  const PortInfo port_info{PortDirection::kDefault, "type_abc"};
  NodePort node_port1(port_info);
  NodePort node_port2(port_info);

  const DoubleClickEvent event1{&node_port1};
  const DoubleClickEvent event2{&node_port2};
  const DoubleClickEvent event3{&node_port2};

  // comparing different events
  EXPECT_FALSE(event1 == event2);
  EXPECT_TRUE(event1 != event2);

  // comparing same events
  EXPECT_TRUE(event1 == event1);
  EXPECT_TRUE(event2 == event3);
  EXPECT_FALSE(event2 != event3);

  // comparing different variants
  EXPECT_FALSE(node_event_t(event1) == node_event_t(event2));
  EXPECT_TRUE(node_event_t(event1) != node_event_t(event2));
}

}  // namespace mvvm::test
