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

#include "mvvm/widgets/item_view_component_provider.h"

#include <mvvm/core/exceptions.h>
#include <mvvm/delegates/viewmodel_delegate.h>
#include <mvvm/model/application_model.h>
#include <mvvm/model/sessionitem.h>
#include <mvvm/viewmodel/viewmodel.h>
#include <mvvm/widgets/item_selection_model.h>

#include <QAbstractItemView>

namespace mvvm
{

ItemViewComponentProvider::ItemViewComponentProvider(create_viewmodel_t model_func,
                                                     QAbstractItemView *view)
    : m_delegate(std::make_unique<mvvm::ViewModelDelegate>())
    , m_selection_model(std::make_unique<mvvm::ItemSelectionModel>())
    , m_create_viewmodel(std::move(model_func))
    , m_view(view)
{
  connect(m_selection_model.get(), &mvvm::ItemSelectionModel::SelectedItemChanged, this,
          [this](auto item) { emit SelectedItemChanged(const_cast<mvvm::SessionItem *>(item)); });
}

ItemViewComponentProvider::~ItemViewComponentProvider() = default;

void ItemViewComponentProvider::SetApplicationModel(mvvm::ApplicationModel *model)
{
  if (!model)
  {
    Reset();
    return;
  }

  InitViewModel(model);
}

void ItemViewComponentProvider::SetItem(mvvm::SessionItem *item)
{
  if (!item)
  {
    Reset();
    return;
  }

  auto application_model = dynamic_cast<mvvm::ApplicationModel *>(item->GetModel());
  if (!application_model)
  {
    throw RuntimeException("Wrong model type");
  }

  InitViewModel(application_model);
  m_view_model->SetRootSessionItem(item);
}

QAbstractItemView *ItemViewComponentProvider::GetView() const
{
  return m_view;
}

mvvm::ItemSelectionModel *ItemViewComponentProvider::GetSelectionModel() const
{
  return m_selection_model.get();
}

mvvm::ViewModel *ItemViewComponentProvider::GetViewModel() const
{
  return m_view_model.get();
}

mvvm::SessionItem *ItemViewComponentProvider::GetSelectedItem() const
{
  return const_cast<mvvm::SessionItem *>(m_selection_model->GetSelectedItem());
}

void ItemViewComponentProvider::SetSelectedItem(mvvm::SessionItem *item)
{
  m_selection_model->SetSelectedItem(item);
}

void ItemViewComponentProvider::SetSelectedItems(std::vector<SessionItem *> items)
{
  std::vector<const mvvm::SessionItem *> to_set_items;
  std::copy(items.begin(), items.end(), std::back_inserter(to_set_items));
  m_selection_model->SetSelectedItems(std::move(to_set_items));
}

void ItemViewComponentProvider::Reset()
{
  m_view->setModel(nullptr);
  m_selection_model->setModel(nullptr);
  m_view_model.reset();
}

void ItemViewComponentProvider::InitViewModel(mvvm::ApplicationModel *model)
{
  m_view_model = m_create_viewmodel(model);
  m_selection_model->SetViewModel(m_view_model.get());
  m_view->setModel(m_view_model.get());
  m_view->setItemDelegate(m_delegate.get());
  m_view->setSelectionModel(m_selection_model.get());
}

std::vector<mvvm::SessionItem *> ItemViewComponentProvider::GetSelectedItemsIntern() const
{
  std::vector<mvvm::SessionItem *> result;
  auto items = m_selection_model->GetSelectedItems();
  std::transform(items.begin(), items.end(), std::back_inserter(result),
                 [](auto it) { return const_cast<mvvm::SessionItem *>(it); });
  return result;
}

}  // namespace mvvm
