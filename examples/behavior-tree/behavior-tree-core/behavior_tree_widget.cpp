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

#include "behavior_tree_widget.h"

#include "graphics_scene.h"
#include "graphics_view.h"

#include <QVBoxLayout>

namespace behaviortree
{

BehaviorTreeWidget::BehaviorTreeWidget(QWidget *parent_widget)
    : m_graphics_scene(new GraphicsScene(this))
    , m_graphics_view(new GraphicsView(m_graphics_scene, this))
{
  auto layout = new QVBoxLayout(this);
  layout->addWidget(m_graphics_view);
}

}  // namespace behaviortree
