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

#include "mvvm/model/abstract_model_decorator.h"
#include "mvvm/model/sessionitem.h"

namespace mvvm
{

AbstractModelDecorator::AbstractModelDecorator(
    std::unique_ptr<SessionModelInterface> decorated_model)
    : m_decorated_model(std::move(decorated_model))
{
}

std::string AbstractModelDecorator::GetType() const
{
  return m_decorated_model->GetType();
}

SessionItem *AbstractModelDecorator::GetRootItem() const
{
  return m_decorated_model->GetRootItem();
}

SessionItem *AbstractModelDecorator::InsertItem(std::unique_ptr<SessionItem> item,
                                                SessionItem *parent, const TagIndex &tag_index)
{
  return m_decorated_model->InsertItem(std::move(item), parent, tag_index);
}

SessionItem *AbstractModelDecorator::InsertNewItem(const std::string &item_type,
                                                   SessionItem *parent, const TagIndex &tag_index)
{
  return m_decorated_model->InsertNewItem(item_type, parent, tag_index);
}

std::unique_ptr<SessionItem> AbstractModelDecorator::TakeItem(SessionItem *parent,
                                                              const TagIndex &tag_index)
{
  return m_decorated_model->TakeItem(parent, tag_index);
}

void AbstractModelDecorator::RemoveItem(SessionItem *item)
{
  m_decorated_model->RemoveItem(item);
}

void AbstractModelDecorator::MoveItem(SessionItem *item, SessionItem *new_parent,
                                      const TagIndex &tag_index)
{
  m_decorated_model->MoveItem(item, new_parent, tag_index);
}

bool AbstractModelDecorator::SetData(SessionItem *item, const variant_t &value, int role)
{
  return m_decorated_model->SetData(item, value, role);
}

void AbstractModelDecorator::Clear(std::function<void(SessionItem *)> callback)
{
  return m_decorated_model->Clear(callback);
}

}  // namespace mvvm
