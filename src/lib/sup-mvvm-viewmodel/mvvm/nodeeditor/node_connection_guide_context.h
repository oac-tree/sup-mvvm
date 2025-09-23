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

#ifndef MVVM_NODEEDITOR_NODE_CONNECTION_GUIDE_CONTEXT_H_
#define MVVM_NODEEDITOR_NODE_CONNECTION_GUIDE_CONTEXT_H_

#include <functional>

namespace mvvm
{

class INodePort;

/**
 * @brief The NodeConnectionGuideContext class provides necessary callbacks for
 * NodeConnectionGuide.
 */
struct NodeConnectionGuideContext
{
  std::function<void()> connection_started;
  std::function<void()> connection_finished;
  std::function<void(const INodePort*, const INodePort*)> connection_request;
};

}  // namespace mvvm

#endif  // MVVM_NODEEDITOR_NODE_CONNECTION_GUIDE_CONTEXT_H_
