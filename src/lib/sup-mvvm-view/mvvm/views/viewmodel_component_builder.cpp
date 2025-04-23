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

#include "viewmodel_component_builder.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/editors/default_editor_factory.h>
#include <mvvm/providers/i_cell_decorator.h>
#include <mvvm/providers/i_editor_factory.h>
#include <mvvm/viewmodel/viewmodel.h>
#include <mvvm/views/default_cell_decorator.h>
#include <mvvm/views/item_view_component_provider.h>
#include <mvvm/views/viewmodel_delegate.h>

namespace mvvm
{

// ------------------------------------------------------------------------------------------------
// ViewModelDelegateBuilder
// ------------------------------------------------------------------------------------------------
ViewModelDelegateBuilder::~ViewModelDelegateBuilder() = default;

ViewModelDelegateBuilder::operator std::unique_ptr<ViewModelDelegate>()
{
  if (!m_cell_decorator)
  {
    m_cell_decorator = std::make_unique<DefaultCellDecorator>();
  }

  if (!m_editor_factory)
  {
    m_editor_factory = std::make_unique<DefaultEditorFactory>();
  }

  return std::make_unique<ViewModelDelegate>(std::move(m_editor_factory),
                                             std::move(m_cell_decorator));
}

std::unique_ptr<ViewModelDelegate> ViewModelDelegateBuilder::Build()
{
  return *this;  // invokes implicit conversion via operator
}

// ------------------------------------------------------------------------------------------------
// ItemViewComponentProviderBuilder
// ------------------------------------------------------------------------------------------------
ItemViewComponentProviderBuilder::~ItemViewComponentProviderBuilder() = default;

ViewModelDelegateBuilder &ItemViewComponentProviderBuilder::Delegate()
{
  return m_viewmodel_delegate_builder;
}

ItemViewComponentProviderBuilder &ItemViewComponentProviderBuilder::View(QAbstractItemView *view)
{
  m_view = view;
  return *this;
}

ItemViewComponentProviderBuilder::operator std::unique_ptr<ItemViewComponentProvider>()
{
  m_viewmodel_delegate = m_viewmodel_delegate_builder.Build();
  return std::make_unique<ItemViewComponentProvider>(std::move(m_viewmodel_delegate),
                                                     std::move(m_viewmodel), m_view);
}

std::unique_ptr<ItemViewComponentProvider> ItemViewComponentProviderBuilder::Build()
{
  return *this;  // invokes implicit conversion via operator
}

ItemViewComponentProviderBuilder CreateProvider()
{
  return ItemViewComponentProviderBuilder();
}

}  // namespace mvvm
