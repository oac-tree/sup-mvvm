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

#include "mock_node_connection_controller_context.h"

namespace mvvm::test
{

NodeConnectionGuideContext MockNodeConnectionGuideContext::CreateContext()
{
  NodeConnectionGuideContext result;
  result.connection_started = [this]() { OnConnectionStarted(); };
  result.connection_finished = [this]() { OnConnectionFinished(); };
  result.connection_request = [this](const INodePort* start, const INodePort* end)
  { OnConnectionRequest(start, end); };

  return result;
}

}  // namespace mvvm::test
