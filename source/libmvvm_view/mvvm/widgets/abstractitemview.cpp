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

AbstractItemView::AbstractItemView(create_viewmodel_t func, QAbstractItemView *view,
                                   ApplicationModel *model, QWidget *parent)
    : ItemViewBase(parent), m_create_viewmodel(std::move(func))
{
  SetView(view);
  SetApplicationModel(model);
}

AbstractItemView::AbstractItemView(std::unique_ptr<ItemViewComponentProvider> provider,
                                   QWidget *parent)

{
}

AbstractItemView::~AbstractItemView() = default;

void AbstractItemView::SetApplicationModel(ApplicationModel *model)
{
  if (!model)
  {
    Reset();
    return;
  }
  SetViewModel(m_create_viewmodel(model));
}

void AbstractItemView::SetItem(SessionItem *item)
{
  if (!item)
  {
    Reset();
    return;
  }

  auto application_model = dynamic_cast<ApplicationModel *>(item->GetModel());
  if (!application_model)
  {
    throw RuntimeException("Wrong model type");
  }
  SetViewModel(m_create_viewmodel(application_model));
  SetRootSessionItem(item);

  if (auto view = dynamic_cast<QTreeView *>(GetView()); view)
  {
    view->expandAll();
    view->resizeColumnToContents(0);
  }
}

AbstractItemViewV2::AbstractItemViewV2(std::unique_ptr<ItemViewComponentProvider> provider,
                                       QWidget *parent)
    : m_provider(std::move(provider))
{
  auto layout = new QVBoxLayout(this);
  layout->setMargin(0);
  layout->setSpacing(0);
}

AbstractItemViewV2::~AbstractItemViewV2() = default;

void AbstractItemViewV2::SetApplicationModel(ApplicationModel *model)
{
  m_provider->SetApplicationModel(model);
}

void AbstractItemViewV2::SetItem(SessionItem *item)
{
  m_provider->SetItem(item);
}

ItemViewComponentProvider *AbstractItemViewV2::GetComponentProvider()
{
  return m_provider.get();
}

}  // namespace mvvm
