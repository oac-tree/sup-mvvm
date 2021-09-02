/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/viewmodel/modeleventnotifier.h"

#include "mvvm/interfaces/modeleventlistenerinterface.h"
#include "mvvm/model/sessionitem.h"

namespace ModelView
{
void ModelEventNotifier::EstablishConnections(ModelEventListenerInterface *listener)
{
  auto on_about_to_insert = [listener](auto parent, auto tag_index)
  { listener->OnAboutToInsertItem(parent, tag_index); };
  connect(this, &ModelEventNotifier::InvokeAboutToInsertItem, on_about_to_insert);

  auto on_inserted = [listener](auto parent, auto tag_index)
  { listener->OnItemInserted(parent, tag_index); };
  connect(this, &ModelEventNotifier::InvokeItemInserted, on_inserted);

  auto on_about_to_remove = [listener](auto parent, auto tag_index)
  { listener->OnAboutToRemoveItem(parent, tag_index); };
  connect(this, &ModelEventNotifier::InvokeAboutToRemoveItem, on_about_to_remove);

  auto on_removed = [listener](auto parent, auto tag_index)
  { listener->OnItemRemoved(parent, tag_index); };
  connect(this, &ModelEventNotifier::InvokeItemRemoved, on_removed);

  auto on_data_changed = [listener](auto item, int role) { listener->OnDataChanged(item, role); };
  connect(this, &ModelEventNotifier::InvokeDataChanged, on_data_changed);
}

void ModelEventNotifier::AboutToInsertItem(SessionItem *parent, const TagIndex &tag_index)
{
  emit InvokeAboutToInsertItem(parent, tag_index);
}

void ModelEventNotifier::ItemInserted(SessionItem *parent, const TagIndex &tag_index)
{
  emit InvokeItemInserted(parent, tag_index);
}

void ModelEventNotifier::AboutToRemoveItem(SessionItem *parent, const TagIndex &tag_index)
{
  emit InvokeAboutToRemoveItem(parent, tag_index);
}

void ModelEventNotifier::ItemRemoved(SessionItem *parent, const TagIndex &tag_index)
{
  emit InvokeItemRemoved(parent, tag_index);
}

void ModelEventNotifier::DataChanged(SessionItem *item, int role)
{
  emit InvokeDataChanged(item, role);
}

}  // namespace ModelView
