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

#ifndef MVVM_VIEWMODEL_ABSTRACTVIEWMODELCONTROLLER_H
#define MVVM_VIEWMODEL_ABSTRACTVIEWMODELCONTROLLER_H

#include "mvvm/interfaces/modeleventlistenerinterface.h"
#include "mvvm/signals/signalslot.h"
#include "mvvm/viewmodel_export.h"

#include <QStringList>

namespace mvvm
{
class SessionModel;
class ModelEventNotifierInterface;

//! Propagate changes

class MVVM_VIEWMODEL_EXPORT AbstractViewModelController : public ModelEventListenerInterface
{
public:
  ~AbstractViewModelController() override;

  void SubscribeTo(ModelEventSubscriberInterface *subscriber) override;
  void UnsubscribeFrom(ModelEventSubscriberInterface *subscriber) override;

  //! Lets the controller know that a child is about to be inserted into the `parent` with
  //! `tag_index`.
  virtual void OnAboutToInsertItem(SessionItem *parent, const TagIndex &tag_index);

  //! Lets the controller know that a child has been inserted into the `parent` with `tag_index`.
  virtual void OnItemInserted(SessionItem *parent, const TagIndex &tag_index);

  //! Lets the controller know that a child is about to be removed from the `parent`s position
  //! `tag_index`.
  virtual void OnAboutToRemoveItem(SessionItem *parent, const TagIndex &tag_index);

  //! Lets the controller know that a child has been removed from the `parent`s position
  //! `tag_index`.
  virtual void OnItemRemoved(SessionItem *parent, const TagIndex &tag_index);

  //! Lets the controller know that `item`s data with given `role` has been changed.
  virtual void OnDataChanged(SessionItem *item, int role);

  //! Lets the controller know when the root item is about to be reset.
  virtual void OnModelAboutToBeReset(SessionModel *model);

  //! Lets the controller know at the end of root item recreation.
  virtual void OnModelReset(SessionModel *model);

  //! Lets the controller know at the beginning of model destruction.
  virtual void OnModelAboutToBeDestroyed(SessionModel *model);

  virtual void Init(SessionItem *root_item = nullptr);

  virtual QStringList GetHorizontalHeaderLabels() const;

private:
  std::unique_ptr<mvvm::Slot> m_slot;
};

}  // namespace mvvm

#endif  // MVVM_VIEWMODEL_ABSTRACTVIEWMODELCONTROLLER_H
