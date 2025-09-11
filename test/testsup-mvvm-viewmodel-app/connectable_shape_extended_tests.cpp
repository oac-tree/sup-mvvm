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

#include "mvvm/nodeeditor/connectable_shape.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/model/application_model.h>
#include <mvvm/nodeeditor/node_item_connectable_shape_adapter.h>
#include <mvvm/nodeeditor/notifying_connectable_shape_adapter.h>
#include <mvvm/nodeeditor/top_bottom_port_position_strategy.h>
#include <mvvm/standarditems/node_item.h>
#include <mvvm/test/mock_item_listener.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <QGraphicsScene>

namespace mvvm::test
{

/**
 * @brief Tests for ConnectableShape class, when view is fully constructed and belongs to scene.
 */
class ConnectableShapeExtendedTest : public ::testing::Test
{
};

TEST_F(ConnectableShapeExtendedTest, SetViewInitialCoordinatesFromItem) {}

}  // namespace mvvm::test
