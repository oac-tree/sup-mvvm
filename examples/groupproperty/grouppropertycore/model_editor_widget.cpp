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

#include "model_editor_widget.h"

#include "sample_model.h"

#include <mvvm/model/model_utils.h>
#include <mvvm/views/all_items_tree_view.h>
#include <mvvm/views/property_tree_view.h>

#include <QHBoxLayout>

namespace groupproperty
{

ModelEditorWidget::ModelEditorWidget(SampleModel* model, QWidget* parent)
    : QWidget(parent)
    , m_tree_view(new mvvm::AllItemsTreeView)
    , m_property_view(new mvvm::PropertyTreeView)
{
  auto layout = new QHBoxLayout(this);
  layout->addWidget(m_tree_view);
  layout->addWidget(m_property_view);

  SetModel(model);
}

void ModelEditorWidget::SetModel(SampleModel* model)
{
  if (!model)
  {
    return;
  }

  // the tree on the left shows the whole content of the model
  m_tree_view->SetApplicationModel(model);

  // the property editor on the right shows the content of a single ParticleItem
  auto particles = mvvm::utils::GetTopItems<ParticleItem>(*model);
  if (!particles.empty())
  {
    m_property_view->SetItem(particles.front());
  }
}

}  // namespace groupproperty
