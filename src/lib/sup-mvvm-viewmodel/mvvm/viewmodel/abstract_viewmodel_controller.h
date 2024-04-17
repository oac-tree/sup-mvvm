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

#ifndef MVVM_VIEWMODEL_ABSTRACT_VIEWMODEL_CONTROLLER_H_
#define MVVM_VIEWMODEL_ABSTRACT_VIEWMODEL_CONTROLLER_H_

#include <mvvm/viewmodel/i_viewmodel_controller.h>

#include <memory>

namespace mvvm
{

class SessionModelInterface;

template <typename T>
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

  const SessionModelInterface* GetModel() const;

  void SetModel(SessionModelInterface* model);

  void OnModelEvent(const AboutToInsertItemEvent& event) override;

  void OnModelEvent(const ItemInsertedEvent& event) override;

  void OnModelEvent(const AboutToRemoveItemEvent& event) override;

  void OnModelEvent(const ItemRemovedEvent& event) override;

  void OnModelEvent(const DataChangedEvent& event) override;

  void OnModelEvent(const ModelAboutToBeResetEvent& event) override;

  void OnModelEvent(const ModelResetEvent& event) override;

  void OnModelEvent(const ModelAboutToBeDestroyedEvent& event) override;

  void SetRootItem(SessionItem* root_item) final;

  QStringList GetHorizontalHeaderLabels() const override;

protected:
  /**
   * @brief Convenience method that subscribes to all signals.
   */
  void SubscribeAll(SessionModelInterface* model);

private:
  virtual void SetRootItemImpl(SessionItem* root_item) = 0;

  /**
   * @brief Subscribe to the model notifications.
   */
  void Subscribe(SessionModelInterface* model);

  /**
   * @brief Implementation to subscribe to the model notifications.
   *
   * @note Should be reimplemented in the derived class (non-virtual interface idiom).
   */
  virtual void SubscribeImpl(SessionModelInterface* model);

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

  std::unique_ptr<ModelListener<SessionModelInterface>> m_listener;
};

}  // namespace mvvm

#endif  // MVVM_VIEWMODEL_ABSTRACT_VIEWMODEL_CONTROLLER_H_
