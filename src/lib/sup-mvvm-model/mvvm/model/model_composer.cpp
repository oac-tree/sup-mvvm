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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "model_composer.h"

#include "session_item.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/model/i_session_model.h>

namespace mvvm
{

ModelComposer::ModelComposer(ISessionModel &model) : m_model(model) {}

SessionItem *ModelComposer::InsertItem(std::unique_ptr<SessionItem> item, SessionItem *parent,
                                       const TagIndex &tag_index)
{
  return parent->InsertItem(std::move(item), tag_index);
}

std::unique_ptr<SessionItem> ModelComposer::TakeItem(SessionItem *parent, const TagIndex &tag_index)
{
  return parent->TakeItem(tag_index);
}

bool ModelComposer::SetData(SessionItem *item, const variant_t &value, int role)
{
  return item->SetData(value, role, /*direct*/ true);
}

void ModelComposer::ReplaceRootItem(std::unique_ptr<SessionItem> &old_root_item,
                                    std::unique_ptr<SessionItem> new_root_item)
{
  if (!new_root_item)
  {
    throw RuntimeException("Attempt to set uninitialised root item.");
  }

  old_root_item = std::move(new_root_item);
  old_root_item->SetModel(&m_model);
}

ISessionModel *ModelComposer::GetModel() const
{
  return &m_model;
}

}  // namespace mvvm
