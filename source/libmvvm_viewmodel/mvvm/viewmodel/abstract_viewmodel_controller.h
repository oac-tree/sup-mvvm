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

#ifndef MVVM_VIEWMODEL_ABSTRACT_VIEWMODEL_CONTROLLER_H_
#define MVVM_VIEWMODEL_ABSTRACT_VIEWMODEL_CONTROLLER_H_

#include <mvvm/signals/event_types.h>
#include <mvvm/signals/signal_slot.h>
#include <mvvm/viewmodel_export.h>

#include <QStringList>

namespace mvvm
{
class SessionModelInterface;
class SessionItem;
class ModelEventHandler;
class TagIndex;

//! Propagate changes

class MVVM_VIEWMODEL_EXPORT AbstractViewModelController
{
public:
  virtual ~AbstractViewModelController();

  void Subscribe(ModelEventHandler *event_handler);
  void Unsubscribe();

  void OnEvent(const event_variant_t &event);

  //! Lets the controller know that a child is about to be inserted into the parent.
  virtual void OnAboutToInsertItem(const AboutToInsertItemEvent& event);

  //! Lets the controller know that a child has been inserted into the parent.
  virtual void OnItemInserted(const ItemInsertedEvent& event);

  //! Lets the controller know that a child is about to be removed from the `parent`s position
  //! `tag_index`.
  virtual void OnAboutToRemoveItem(const AboutToRemoveItemEvent& event);

  //! Lets the controller know that a child has been removed from the `parent`s position
  //! `tag_index`.
  virtual void OnItemRemoved(const ItemRemovedEvent& event);

  //! Lets the controller know that `item`s data with given `role` has been changed.
  virtual void OnDataChanged(const DataChangedEvent& event);

  //! Lets the controller know when the root item is about to be reset.
  virtual void OnModelAboutToBeReset(const ModelAboutToBeResetEvent& event);

  //! Lets the controller know at the end of root item recreation.
  virtual void OnModelReset(const ModelResetEvent& event);

  //! Lets the controller know at the beginning of model destruction.
  virtual void OnModelAboutToBeDestroyed(SessionModelInterface *model);

  virtual void Init(SessionItem *root_item = nullptr);

  virtual QStringList GetHorizontalHeaderLabels() const;

  void operator()(const DataChangedEvent &event);
  void operator()(const AboutToInsertItemEvent &event);
  void operator()(const ItemInsertedEvent &event);
  void operator()(const AboutToRemoveItemEvent &event);
  void operator()(const ItemRemovedEvent &event);
  void operator()(const ModelAboutToBeResetEvent &event);
  void operator()(const ModelResetEvent &event);
  void operator()(const ModelAboutToBeDestroyedEvent &event);

private:
  std::unique_ptr<mvvm::Slot> m_slot;
};

}  // namespace mvvm

#endif  // MVVM_VIEWMODEL_ABSTRACT_VIEWMODEL_CONTROLLER_H_
