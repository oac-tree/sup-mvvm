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

#include "mvvm/signals/modeleventnotifier.h"

#include "mvvm/model/sessionitem.h"
#include "mvvm/model/sessionmodel.h"
#include "mvvm/signals/signalslot.h"

namespace mvvm
{
struct ModelEventNotifier::ModelEventNotifierImpl
{
  Signal<void(SessionItem *, const TagIndex &)> m_about_to_insert_item;
  Signal<void(SessionItem *, const TagIndex &)> m_item_inserted;
  Signal<void(SessionItem *, const TagIndex &)> m_about_to_remove_item;
  Signal<void(SessionItem *, const TagIndex &)> m_item_removed;
  Signal<void(SessionItem *, int)> m_data_changed;
  Signal<void(SessionModel *)> m_model_about_to_reset;
  Signal<void(SessionModel *)> m_model_reset;
  Signal<void(SessionModel *)> m_model_about_to_be_destroyed;
};

ModelEventNotifier::ModelEventNotifier() : p_impl(std::make_unique<ModelEventNotifierImpl>()) {}

ModelEventNotifier::~ModelEventNotifier() = default;

Connection ModelEventNotifier::SetOnAboutToInsertItem(const Callbacks::item_tagindex_t &f,
                                                      Slot *slot)
{
  return p_impl->m_about_to_insert_item.connect(f, slot);
}

Connection ModelEventNotifier::SetOnItemInserted(const Callbacks::item_tagindex_t &f, Slot *slot)
{
  return p_impl->m_item_inserted.connect(f, slot);
}

Connection ModelEventNotifier::SetOnAboutToRemoveItem(const Callbacks::item_tagindex_t &f,
                                                      Slot *slot)
{
  return p_impl->m_about_to_remove_item.connect(f, slot);
}

Connection ModelEventNotifier::SetOnItemRemoved(const Callbacks::item_tagindex_t &f, Slot *slot)
{
  return p_impl->m_item_removed.connect(f, slot);
}

Connection ModelEventNotifier::SetOnDataChanged(const Callbacks::item_int_t &f, Slot *slot)
{
  return p_impl->m_data_changed.connect(f, slot);
}

Connection ModelEventNotifier::SetOnModelAboutToBeReset(const Callbacks::model_t &f, Slot *slot)
{
  return p_impl->m_model_about_to_reset.connect(f, slot);
}

Connection ModelEventNotifier::SetOnModelReset(const Callbacks::model_t &f, Slot *slot)
{
  return p_impl->m_model_reset.connect(f, slot);
}

Connection ModelEventNotifier::SetOnModelAboutToBeDestroyed(const Callbacks::model_t &f, Slot *slot)
{
  return p_impl->m_model_about_to_be_destroyed.connect(f, slot);
}

// ------------------------------------------------------------------------

void ModelEventNotifier::AboutToInsertItemNotify(SessionItem *parent, const TagIndex &tag_index)
{
  p_impl->m_about_to_insert_item(parent, tag_index);
}

void ModelEventNotifier::ItemInsertedNotify(SessionItem *parent, const TagIndex &tag_index)
{
  p_impl->m_item_inserted(parent, tag_index);
}

void ModelEventNotifier::AboutToRemoveItemNotify(SessionItem *parent, const TagIndex &tag_index)
{
  p_impl->m_about_to_remove_item(parent, tag_index);
}

void ModelEventNotifier::ItemRemovedNotify(SessionItem *parent, const TagIndex &tag_index)
{
  p_impl->m_item_removed(parent, tag_index);
}

void ModelEventNotifier::DataChangedNotify(SessionItem *item, int role)
{
  p_impl->m_data_changed(item, role);
}

void ModelEventNotifier::ModelAboutToBeResetNotify(SessionModel *model)
{
  p_impl->m_model_about_to_reset(model);
}

void ModelEventNotifier::ModelResetNotify(SessionModel *model)
{
  p_impl->m_model_reset(model);
}

void ModelEventNotifier::ModelAboutToBeDestroyedNotify(SessionModel *model)
{
  p_impl->m_model_about_to_be_destroyed(model);
}

}  // namespace mvvm
