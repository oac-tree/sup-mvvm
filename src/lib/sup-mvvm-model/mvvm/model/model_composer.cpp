/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
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

#include "model_utils.h"
#include "sessionitem.h"

#include <mvvm/interfaces/sessionmodel_interface.h>

namespace mvvm
{

ModelComposer::ModelComposer(SessionModelInterface &model) : m_model(model) {}

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

void ModelComposer::Reset(std::unique_ptr<SessionItem> &old_root_item,
                          std::unique_ptr<SessionItem> new_root_item)
{
  old_root_item = new_root_item ? std::move(new_root_item) : utils::CreateEmptyRootItem();
  old_root_item->SetModel(&m_model);
}

SessionModelInterface *ModelComposer::GetModel() const
{
  return &m_model;
}

}  // namespace mvvm
