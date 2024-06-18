/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_VIEWMODEL_I_VIEWMODEL_CONTROLLER_H_
#define MVVM_VIEWMODEL_I_VIEWMODEL_CONTROLLER_H_

#include <mvvm/signals/event_types.h>
#include <mvvm/viewmodel_export.h>

#include <QStringList>

namespace mvvm
{

class ISessionModel;

/**
 * @brief The IViewModelController class is a base class for all ViewModel controllers.
 *
 * Its main purpose is to listen SessionModel and make certain changes in a viewmodel.
 */
class MVVM_VIEWMODEL_EXPORT IViewModelController
{
public:
  virtual ~IViewModelController() = default;

  /**
   * @brief Lets the controller know that a child is about to be inserted into the parent.
   */
  virtual void OnModelEvent(const AboutToInsertItemEvent& event) = 0;

  /**
   * @brief Lets the controller know that a child has been inserted into the parent.
   */
  virtual void OnModelEvent(const ItemInsertedEvent& event) = 0;

  /**
   * @brief Let's the controller know that an item is about to be removed.
   */
  virtual void OnModelEvent(const AboutToRemoveItemEvent& event) = 0;

  /**
   * @brief OnModelEventLet's the controller know that item has been removed.
   * @param event
   */
  virtual void OnModelEvent(const ItemRemovedEvent& event) = 0;

  /**
   * @brief Lets the controller know thatitem's data has been changed.
   */
  virtual void OnModelEvent(const DataChangedEvent& event) = 0;

  /**
   * @brief Lets the controller know when the root item is about to be reset.
   */
  virtual void OnModelEvent(const ModelAboutToBeResetEvent& event) = 0;

  /**
   * @brief Lets the controller know at the end of root item recreation.
   */
  virtual void OnModelEvent(const ModelResetEvent& event) = 0;

  /**
   * @brief Lets the controller know at the beginning of model destruction.
   */
  virtual void OnModelEvent(const ModelAboutToBeDestroyedEvent& event) = 0;

  /**
   * @brief Returns current root item.
   */
  virtual const SessionItem* GetRootItem() const = 0;

  /**
   * @brief Sets an item as a new root item.
   */
  virtual void SetRootItem(SessionItem* root_item) = 0;

  /**
   * @brief Returns number of columns.
   */
  virtual int GetColumnCount() const = 0;

  /**
   * @brief Returns list representing horizontal labels.
   */
  virtual QStringList GetHorizontalHeaderLabels() const = 0;
};

}  // namespace mvvm

#endif  // MVVM_VIEWMODEL_I_VIEWMODEL_CONTROLLER_H_
