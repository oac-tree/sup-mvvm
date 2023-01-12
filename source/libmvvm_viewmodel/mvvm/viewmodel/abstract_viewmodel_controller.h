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
#include <mvvm/viewmodel_export.h>

#include <QStringList>
#include <memory>

namespace mvvm
{

class SessionModelInterface;

//! Propagate changes

class MVVM_VIEWMODEL_EXPORT AbstractViewModelController
{
public:
  AbstractViewModelController();
  virtual ~AbstractViewModelController();

  AbstractViewModelController(const AbstractViewModelController& other) = delete;
  AbstractViewModelController& operator=(const AbstractViewModelController& other) = delete;

  void Subscribe(SessionModelInterface* model);
  void Unsubscribe();

  //! Lets the controller know that a child is about to be inserted into the parent.
  virtual void OnModelEvent(const AboutToInsertItemEvent& event);

  //! Lets the controller know that a child has been inserted into the parent.
  virtual void OnModelEvent(const ItemInsertedEvent& event);

  //! Lets the controller know that a child is about to be removed from the `parent`s position
  //! `tag_index`.
  virtual void OnModelEvent(const AboutToRemoveItemEvent& event);

  //! Lets the controller know that a child has been removed from the `parent`s position
  //! `tag_index`.
  virtual void OnModelEvent(const ItemRemovedEvent& event);

  //! Lets the controller know that `item`s data with given `role` has been changed.
  virtual void OnModelEvent(const DataChangedEvent& event);

  //! Lets the controller know when the root item is about to be reset.
  virtual void OnModelEvent(const ModelAboutToBeResetEvent& event);

  //! Lets the controller know at the end of root item recreation.
  virtual void OnModelEvent(const ModelResetEvent& event);

  //! Lets the controller know at the beginning of model destruction.
  virtual void OnModelEvent(const ModelAboutToBeDestroyedEvent& event);

  virtual void Init(SessionItem* root_item = nullptr);

  virtual QStringList GetHorizontalHeaderLabels() const;

private:
  struct AbstractViewModelControllerImpl;
  std::unique_ptr<AbstractViewModelControllerImpl> p_impl;
};

}  // namespace mvvm

#endif  // MVVM_VIEWMODEL_ABSTRACT_VIEWMODEL_CONTROLLER_H_
