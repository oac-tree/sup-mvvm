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

#ifndef MVVM_INTERFACES_MODELEVENTLISTENERINTERFACE_H
#define MVVM_INTERFACES_MODELEVENTLISTENERINTERFACE_H

#include "mvvm/model/function_types.h"
#include "mvvm/model_export.h"

namespace mvvm
{
class SessionItem;
class TagIndex;
class ModelEventNotifierInterface;
class SessionModel;

//! Interface for all objects that need to listen for events happening with the SessionModel.

class MVVM_MODEL_EXPORT ModelEventListenerInterface
{
public:
  virtual ~ModelEventListenerInterface() = default;

  //! Inform the listener about serving notifier.
  virtual void SubscribeTo(ModelEventNotifierInterface* notifier) = 0;

  //! Inform the listener about serving notifier.
  virtual void SetNotifier(ModelEventNotifierInterface* notifier) = 0;

  //! Lets the listener know that a child is about to be inserted into the `parent` with
  //! `tag_index`.
  virtual void OnAboutToInsertItem(SessionItem* parent, const TagIndex& tag_index) = 0;

  //! Lets the listener know that a child has been inserted into the `parent` with `tag_index`.
  virtual void OnItemInserted(SessionItem* parent, const TagIndex& tag_index) = 0;

  //! Lets the listener know that a child is about to be removed from the `parent`s position
  //! `tag_index`.
  virtual void OnAboutToRemoveItem(SessionItem* parent, const TagIndex& tag_index) = 0;

  //! Lets the listener know that a child has been removed from the `parent`s position
  //! `tag_index`.
  virtual void OnItemRemoved(SessionItem* parent, const TagIndex& tag_index) = 0;

  //! Lets the listener know that `item`s data with given `role` has been changed.
  virtual void OnDataChanged(SessionItem* item, int role) = 0;

  //! Lets the listener know when the root item is about to be reset.
  virtual void OnModelAboutToBeReset(SessionModel* model) = 0;

  //! Lets the listener know at the end of root item recreation.
  virtual void OnModelReset(SessionModel* model) = 0;

  //! Lets the listener know at the beginning of model destruction.
  virtual void OnModelAboutToBeDestroyed(SessionModel* model) = 0;
};

}  // namespace mvvm

#endif  // MVVM_INTERFACES_MODELEVENTLISTENERINTERFACE_H
