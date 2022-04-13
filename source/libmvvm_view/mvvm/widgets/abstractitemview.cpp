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
#include "mvvm/delegates/viewmodeldelegate.h"
#include "mvvm/model/sessionitem.h"
#include "mvvm/viewmodel/viewmodel.h"
#include "mvvm/widgets/itemselectionmodel.h"

#include <QAbstractItemView>
#include <QVBoxLayout>

namespace mvvm
{

AbstractItemView::AbstractItemView(QWidget *parent)
    : QWidget(parent)
    , m_delegate(std::make_unique<ViewModelDelegate>())
    , m_selection_model(std::make_unique<ItemSelectionModel>())

{
  auto layout = new QVBoxLayout(this);
  layout->setMargin(0);
  layout->setSpacing(0);
}

//! Set given view for use. The ownership will be changed.

void AbstractItemView::SetView(QAbstractItemView *view)
{
  m_view = view;
  layout()->addWidget(view);
}

void AbstractItemView::SetViewModel(std::unique_ptr<ViewModel> view_model)
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

  connect(m_selection_model.get(), &ItemSelectionModel::SelectedItemChanged, this,
          &AbstractItemView::SelectedItemChanged, Qt::UniqueConnection);
}

ItemSelectionModel *AbstractItemView::GetSelectionModel() const
{
  return m_selection_model.get();
}

AbstractItemView::~AbstractItemView() = default;

}  // namespace mvvm
