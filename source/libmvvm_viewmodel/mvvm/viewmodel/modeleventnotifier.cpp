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
ModelEventNotifier::ModelEventNotifier(ModelEventListenerInterface *listener)
{
  if (listener)
  {
    EstablishConnections(listener);
  }
}

void ModelEventNotifier::EstablishConnections(ModelEventListenerInterface *listener)
{
  auto on_about_to_insert = [listener](auto parent, auto tag_index)
  { listener->OnAboutToInsertItem(parent, tag_index); };
  connect(this, &ModelEventNotifier::AboutToInsertItem, on_about_to_insert);

  auto on_inserted = [listener](auto parent, auto tag_index)
  { listener->OnItemInserted(parent, tag_index); };
  connect(this, &ModelEventNotifier::ItemInserted, on_inserted);

  auto on_about_to_remove = [listener](auto parent, auto tag_index)
  { listener->OnAboutToRemoveItem(parent, tag_index); };
  connect(this, &ModelEventNotifier::AboutToRemoveItem, on_about_to_remove);

  auto on_removed = [listener](auto parent, auto tag_index)
  { listener->OnItemRemoved(parent, tag_index); };
  connect(this, &ModelEventNotifier::ItemRemoved, on_removed);

  auto on_data_changed = [listener](auto item, int role) { listener->OnDataChanged(item, role); };
  connect(this, &ModelEventNotifier::DataChanged, on_data_changed);
}

void ModelEventNotifier::AboutToInsertItemNotify(SessionItem *parent, const TagIndex &tag_index)
{
  emit AboutToInsertItem(parent, tag_index);
}

void ModelEventNotifier::ItemInsertedNotify(SessionItem *parent, const TagIndex &tag_index)
{
  emit ItemInserted(parent, tag_index);
}

void ModelEventNotifier::AboutToRemoveItemNotify(SessionItem *parent, const TagIndex &tag_index)
{
  emit AboutToRemoveItem(parent, tag_index);
}

void ModelEventNotifier::ItemRemovedNotify(SessionItem *parent, const TagIndex &tag_index)
{
  emit ItemRemoved(parent, tag_index);
}

void ModelEventNotifier::DataChangedNotify(SessionItem *item, int role)
{
  emit DataChanged(item, role);
}

}  // namespace ModelView
