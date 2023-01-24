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

#ifndef MVVM_SIGNALS_EVENT_TYPES_H_
#define MVVM_SIGNALS_EVENT_TYPES_H_

//! @file event_types.h
//! Defines collection of event types.

#include <mvvm/model/tagindex.h>

#include <variant>

namespace mvvm
{

class SessionItem;
class SessionModelInterface;

//! An event when item's data has changed.

struct DataChangedEvent
{
  SessionItem* m_item{nullptr};  //! item whose data has changed
  int m_data_role{0};            //! the role associated with the data

  bool operator==(const DataChangedEvent& other) const;
  bool operator!=(const DataChangedEvent& other) const;
};

//! An event when an item's property has changed.
//! An item's property is another child item added with CompoundItem::AddProperty(name) command.
//! The change of the data of this child will trigger PropertyChangedEvent for it's parent.

struct PropertyChangedEvent
{
  SessionItem* m_item{nullptr};  //! item whose property has changed
  std::string m_name;            //! the name of the changed property

  bool operator==(const PropertyChangedEvent& other) const;
  bool operator!=(const PropertyChangedEvent& other) const;
};

//! An event when item is about to be inserted in the model. It reports the parent, and address of
//! the child.

struct AboutToInsertItemEvent
{
  SessionItem* m_item{nullptr};  //! item that is about to get a new child
  TagIndex m_tag_index;          //! position of a child

  bool operator==(const AboutToInsertItemEvent& other) const;
  bool operator!=(const AboutToInsertItemEvent& other) const;
};

//! An event when item was inserted in the model. It reports the parent, and address of
//! the child.

struct ItemInsertedEvent
{
  SessionItem* m_item{nullptr};  //! item that got a new child (i.e. parent)
  TagIndex m_tag_index;          //! position of a child

  bool operator==(const ItemInsertedEvent& other) const;
  bool operator!=(const ItemInsertedEvent& other) const;
};

//! An event when item is about to be removed from the model. It reports the parent, and address of
//! the child.

struct AboutToRemoveItemEvent
{
  SessionItem* m_item{nullptr};  //! item whose child is about to be removed
  TagIndex m_tag_index;          //! position of a child

  bool operator==(const AboutToRemoveItemEvent& other) const;
  bool operator!=(const AboutToRemoveItemEvent& other) const;
};

//! An event when item was removed from the model. It reports the parent, and address of
//! the child.

struct ItemRemovedEvent
{
  SessionItem* m_item{nullptr};  //! item whose child was removed
  TagIndex m_tag_index;          //! position of a child

  bool operator==(const ItemRemovedEvent& other) const;
  bool operator!=(const ItemRemovedEvent& other) const;
};

//! An event when the root item of the model is about to be reset.

struct ModelAboutToBeResetEvent
{
  SessionModelInterface* m_model{nullptr};  //! the model being reset

  bool operator==(const ModelAboutToBeResetEvent& other) const;
  bool operator!=(const ModelAboutToBeResetEvent& other) const;
};

//! An event at the end of root item reset.

struct ModelResetEvent
{
  SessionModelInterface* m_model{nullptr};  //! the model being reset

  bool operator==(const ModelResetEvent& other) const;
  bool operator!=(const ModelResetEvent& other) const;
};

//! An event at the beginning of the model destruction.

struct ModelAboutToBeDestroyedEvent
{
  SessionModelInterface* m_model{nullptr};  //! the model being destroyed

  bool operator==(const ModelAboutToBeDestroyedEvent& other) const;
  bool operator!=(const ModelAboutToBeDestroyedEvent& other) const;
};

//! Variant for all application events.
using event_variant_t =
    std::variant<DataChangedEvent, PropertyChangedEvent, AboutToInsertItemEvent, ItemInsertedEvent,
                 AboutToRemoveItemEvent, ItemRemovedEvent, ModelAboutToBeResetEvent,
                 ModelResetEvent, ModelAboutToBeDestroyedEvent>;

}  // namespace mvvm

#endif  // MVVM_SIGNALS_EVENT_TYPES_H_
