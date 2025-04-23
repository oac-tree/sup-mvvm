/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_VIEWMODEL_ABSTRACT_VIEWMODEL_CONTROLLER_H_
#define MVVM_VIEWMODEL_ABSTRACT_VIEWMODEL_CONTROLLER_H_

#include <mvvm/viewmodel/i_viewmodel_controller.h>

#include <memory>

namespace mvvm
{

class ISessionModel;
class ModelListener;

/**
 * @brief The AbstractViewModelController class partially implements IViewModelController interface
 * and provides the logic to subscribe/unsubscribe from the model.
 */
class MVVM_VIEWMODEL_EXPORT AbstractViewModelController : public IViewModelController
{
public:
  AbstractViewModelController();
  ~AbstractViewModelController() override;

  AbstractViewModelController(const AbstractViewModelController& other) = delete;
  AbstractViewModelController& operator=(const AbstractViewModelController& other) = delete;
  AbstractViewModelController(AbstractViewModelController&& other) = delete;
  AbstractViewModelController& operator=(AbstractViewModelController&&) = delete;

  const ISessionModel* GetModel() const;

  void SetModel(ISessionModel* model);

  void OnModelEvent(const AboutToInsertItemEvent& event) override;

  void OnModelEvent(const ItemInsertedEvent& event) override;

  void OnModelEvent(const AboutToRemoveItemEvent& event) override;

  void OnModelEvent(const ItemRemovedEvent& event) override;

  void OnModelEvent(const DataChangedEvent& event) override;

  void OnModelEvent(const ModelAboutToBeResetEvent& event) override;

  void OnModelEvent(const ModelResetEvent& event) override;

  void OnModelEvent(const ModelAboutToBeDestroyedEvent& event) override;

  void SetRootItem(SessionItem* root_item) final;

  int GetColumnCount() const override;

  QStringList GetHorizontalHeaderLabels() const override;

protected:
  /**
   * @brief Convenience method that subscribes to all signals.
   */
  void SubscribeAll(ISessionModel* model);

private:
  virtual void SetRootItemImpl(SessionItem* root_item) = 0;

  /**
   * @brief Subscribe to the model notifications.
   */
  void Subscribe(ISessionModel* model);

  /**
   * @brief Implementation to subscribe to the model notifications.
   *
   * @note Should be reimplemented in the derived class (non-virtual interface idiom).
   */
  virtual void SubscribeImpl(ISessionModel* model);

  /**
   * @brief Unsubscribe from the model notifications.
   */
  void Unsubscribe();

  /**
   * @brief Implementation to unsubscribe from the model notifications.
   *
   * @note Should be reimplemented in the derived class (non-virtual interface idiom).
   */
  virtual void UnsubscribeImpl();

  std::unique_ptr<ModelListener> m_listener;
};

}  // namespace mvvm

#endif  // MVVM_VIEWMODEL_ABSTRACT_VIEWMODEL_CONTROLLER_H_
