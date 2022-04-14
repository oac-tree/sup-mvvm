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

#include "mvvm/widgets/itemviewbase.h"

#include "mvvm/core/exceptions.h"
#include "mvvm/delegates/viewmodeldelegate.h"
#include "mvvm/model/sessionitem.h"
#include "mvvm/viewmodel/viewmodel.h"
#include "mvvm/widgets/itemselectionmodel.h"

#include <QAbstractItemView>
#include <QVBoxLayout>

namespace mvvm
{

ItemViewBase::ItemViewBase(QWidget *parent)
    : QWidget(parent)
    , m_delegate(std::make_unique<ViewModelDelegate>())
    , m_selection_model(std::make_unique<ItemSelectionModel>())

{
  auto layout = new QVBoxLayout(this);
  layout->setMargin(0);
  layout->setSpacing(0);

  connect(m_selection_model.get(), &ItemSelectionModel::SelectedItemChanged, this,
          [this](auto item) { emit SelectedItemChanged(const_cast<SessionItem *>(item)); });
}

void ItemViewBase::SetRootSessionItem(SessionItem *item)
{
  if (!m_view_model)
  {
    throw ArgumentNullException("View model doesn't exist");
  }
  m_view_model->SetRootSessionItem(item);
}

QAbstractItemView *ItemViewBase::GetView() const
{
  return m_view;
}

//! Set given view for use. The ownership will be changed.

void ItemViewBase::SetView(QAbstractItemView *view)
{
  m_view = view;
  layout()->addWidget(view);
}

void ItemViewBase::SetViewModel(std::unique_ptr<ViewModel> view_model)
{
  if (!m_view)
  {
    throw ArgumentNullException("View doesn't exist");
  }

  m_view_model = std::move(view_model);

  m_selection_model->SetViewModel(m_view_model.get());

  m_view->setModel(m_view_model.get());
  m_view->setItemDelegate(m_delegate.get());
  m_view->setSelectionModel(m_selection_model.get());
}

ViewModel *ItemViewBase::GetViewModel() const
{
  return m_view_model.get();
}

SessionItem *ItemViewBase::GetSelectedItem() const
{
  return const_cast<SessionItem *>(m_selection_model->GetSelectedItem());
}

std::vector<const SessionItem *> ItemViewBase::GetSelectedItems() const
{
  return m_selection_model->GetSelectedItems();
}

void ItemViewBase::SetSelectedItem(SessionItem *item)
{
  m_selection_model->SetSelectedItem(item);
}

void ItemViewBase::SetSelectedItems(std::vector<const SessionItem *> items)
{
  m_selection_model->SetSelectedItems(std::move(items));
}

void ItemViewBase::Reset()
{
  if (m_view)
  {
    m_view->setModel(nullptr);
  }
  m_selection_model->setModel(nullptr);
  m_view_model.reset();
}

ItemViewBase::~ItemViewBase() = default;

}  // namespace mvvm
