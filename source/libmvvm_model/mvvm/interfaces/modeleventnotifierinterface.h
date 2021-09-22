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

#ifndef MVVM_INTERFACES_MODELEVENTNOTIFIERINTERFACE_H
#define MVVM_INTERFACES_MODELEVENTNOTIFIERINTERFACE_H

#include "mvvm/model_export.h"

namespace ModelView
{
class SessionItem;
class TagIndex;
class ModelEventListenerInterface;

//! Interface class for all objects that trigger notifications when some event happens with
//! SessionModel.

class MVVM_MODEL_EXPORT ModelEventNotifierInterface
{
public:
  virtual ~ModelEventNotifierInterface() = default;

  virtual void EstablishConnections(ModelEventListenerInterface* listener) = 0;

  //! Performs notification of all listeners when an item is about to be inserted into a model.
  virtual void AboutToInsertItemNotify(SessionItem* parent, const TagIndex& tag_index) = 0;

  //! Performs notification of all listeners when an item was inserted into a model.
  virtual void ItemInsertedNotify(SessionItem* parent, const TagIndex& tag_index) = 0;

  //! Performs notification of all listeners when an item is about to be removed from a model.
  virtual void AboutToRemoveItemNotify(SessionItem* parent, const TagIndex& tag_index) = 0;

  //! Performs notification of all listeners when an item was removed from a model.
  virtual void ItemRemovedNotify(SessionItem* parent, const TagIndex& tag_index) = 0;

  //! Performs notification of all listeners when item's data has been changed for a given role.
  virtual void DataChangedNotify(SessionItem* item, int role) = 0;
};

}  // namespace ModelView

#endif  // MVVM_INTERFACES_MODELEVENTNOTIFIERINTERFACE_H
