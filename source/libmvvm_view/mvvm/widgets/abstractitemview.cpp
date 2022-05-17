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

#include "mvvm/widgets/abstractitemview.h"

#include "mvvm/core/exceptions.h"
#include "mvvm/model/applicationmodel.h"
#include "mvvm/viewmodel/viewmodel.h"
#include "mvvm/widgets/itemviewcomponentprovider.h"

#include <QTreeView>
#include <QVBoxLayout>

namespace mvvm
{

AbstractItemView::AbstractItemView(QWidget *parent) : QWidget(parent)
{
  auto layout = new QVBoxLayout(this);
  layout->setMargin(0);
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

void AbstractItemView::SetApplicationModel(ApplicationModel *model)
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

void AbstractItemView::SetSelectedItem(SessionItem *item)
{
  m_provider->SetSelectedItem(item);
}

void AbstractItemView::UpdateView()
{
  // view specific update to execute after model is set
}

}  // namespace mvvm
