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

#include "abstract_item_view.h"

#include "item_view_component_provider.h"

#include <mvvm/core/exceptions.h>
#include <mvvm/model/sessionitem.h>
#include <mvvm/viewmodel/viewmodel.h>

#include <QTreeView>
#include <QVBoxLayout>

namespace mvvm
{

AbstractItemView::AbstractItemView(QWidget *parent) : QWidget(parent)
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
}

void AbstractItemView::SetComponentProvider(std::unique_ptr<ItemViewComponentProvider> provider)
{
  m_provider = std::move(provider);
  connect(m_provider.get(), &ItemViewComponentProvider::SelectedItemChanged, this,
          &AbstractItemView::SelectedItemChanged);
  layout()->addWidget(m_provider->GetView());
}

AbstractItemView::~AbstractItemView() = default;

void AbstractItemView::SetApplicationModel(SessionModelInterface *model)
{
  m_provider->SetApplicationModel(model);
  UpdateView();
}

void AbstractItemView::SetItem(SessionItem *item)
{
  m_provider->SetItem(item);
  UpdateView();
}

ItemViewComponentProvider *AbstractItemView::GetComponentProvider()
{
  return m_provider.get();
}

SessionItem *AbstractItemView::GetSelectedItem() const
{
  return m_provider->GetSelectedItem();
}

std::vector<SessionItem *> AbstractItemView::GetSelectedItems() const
{
  return m_provider->GetSelectedItems();
}

void AbstractItemView::SetSelectedItem(SessionItem *item)
{
  m_provider->SetSelectedItem(item);
}

void AbstractItemView::SetSelectedItems(const std::vector<SessionItem *> &items)
{
  m_provider->SetSelectedItems(items);
}

void AbstractItemView::UpdateView()
{
  // view specific update to execute after model is set
}

}  // namespace mvvm
