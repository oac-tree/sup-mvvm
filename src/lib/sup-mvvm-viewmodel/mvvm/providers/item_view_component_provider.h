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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_PROVIDERS_ITEM_VIEW_COMPONENT_PROVIDER_H_
#define MVVM_PROVIDERS_ITEM_VIEW_COMPONENT_PROVIDER_H_

#include <mvvm/utils/container_utils.h>

#include <QItemSelectionModel>
#include <QObject>
#include <memory>

class QAbstractItemView;
class QItemSelectionModel;
class QAbstractProxyModel;

namespace mvvm
{

class SessionItem;
class ViewModel;
class ViewModelDelegate;
class ISessionModel;

/**
 * @brief The ItemViewComponentProvider class provides QAbstractItemView with custom components:
 * viewmodel, delegate and selection model.
 *
 * It owns all components, but doesn't own a view. Provider allows to install additional proxy
 * models, one after another. Proxy model will get original ViewModel as a source, and the view will
 * be set to a proxy model. Every subsequent proxy model will get previous proxy model as a source.
 */
class ItemViewComponentProvider : public QObject
{
  Q_OBJECT

public:
  /**
   * @brief Main c-tor.
   *
   * Setups a view so it gets proper delegate and viewmodel.
   *
   * @param view_model The viewmodel.
   * @param view The view to serve.
   */
  ItemViewComponentProvider(std::unique_ptr<ViewModel> view_model, QAbstractItemView* view);

  /**
   * @brief Main c-tor.
   *
   * Setups a view so it gets proper delegate and viewmodel.
   *
   * @param view_model The viewmodel.
   * @param view_model_delegate The viewmodel delegate.
   * @param view The view to serve.
   */
  ItemViewComponentProvider(std::unique_ptr<ViewModelDelegate> view_model_delegate,
                            std::unique_ptr<ViewModel> view_model,
                            QAbstractItemView* view);

  ~ItemViewComponentProvider() override;

  ItemViewComponentProvider(const ItemViewComponentProvider& other) = delete;
  ItemViewComponentProvider& operator=(const ItemViewComponentProvider&) = delete;
  ItemViewComponentProvider(ItemViewComponentProvider&& other) = delete;
  ItemViewComponentProvider& operator=(ItemViewComponentProvider&&) = delete;

  /**
   * @brief Sets application model.
   *
   * @details Internal viewmodel will be subscribed to model notification and will present a model
   * starting from its root item.
   */
  void SetApplicationModel(ISessionModel* model);

  /**
   * @brief Set an item to be a new invisible root item for view model.
   */
  void SetItem(SessionItem* item);

  /**
   * @brief Returns current invisible root item we are looking at.
   */
  const SessionItem* GetItem() const;

  /**
   * @brief Returns current invisible root item we are looking at.
   */
  SessionItem* GetItem();

  /**
   * @brief Adds proxy model.
   *
   * Proxy model will get original ViewModel as a source, and the view will be set to a proxy model.
   * Every subsequent proxy model will get previous proxy model as a source.
   */
  void AddProxyModel(std::unique_ptr<QAbstractProxyModel> proxy);

  /**
   * @brief Returns last proxy model in a chain.
   *
   * This is the model which view is looking at.
   */
  QAbstractProxyModel* GetLastProxyModel() const;

  /**
   * @brief Returns chain of proxy models.
   *
   * The last model in this list is what view sees. The first model in this list is using
   * mvvm::ViewModel as a source.
   */
  std::vector<QAbstractProxyModel*> GetProxyModelChain() const;

  /**
   * @brief Returns a view which we are currently serving.
   */
  QAbstractItemView* GetView() const;

  /**
   * @brief Returns used selection model.
   */
  QItemSelectionModel* GetSelectionModel() const;

  /**
   * @brief Returns a view model.
   */
  ViewModel* GetViewModel() const;

  /**
   * @brief Returns item from view index.
   *
   * The method takes into account the existence of proxy model. When proxy model is set, the result
   * is obtained by applying QAbstractItemProxyModel::mapToSource to given index, and then getting
   * item out of it.
   */
  const mvvm::SessionItem* GetItemFromViewIndex(const QModelIndex& index) const;

  /**
   * @brief Returns view indices for given item.
   *
   * View indices can be used for operation through a view. The method takes into account
   * the existence of proxy model. When proxy model is set, the result is obtained by applying
   * QAbstractItemProxyModel::mapFromSource to the original item's indices.
   */
  QList<QModelIndex> GetViewIndexes(const mvvm::SessionItem* item) const;

  /**
   * @brief Returns an item currently selected in a view.
   */
  SessionItem* GetSelectedItem() const;

  /**
   * @brief Make given item selected in a view.
   */
  void SetSelectedItem(SessionItem* item);

  /**
   * @brief Returns an item currently selected in a view and casted to specified type.
   */
  template <typename T>
  T* GetSelected() const;

  /**
   * @brief Returns items currently selected in a view and casted to specified type.
   */
  template <typename T = SessionItem>
  std::vector<T*> GetSelectedItems() const;

  /**
   * @brief Makes all specified items selected in a view.
   */
  void SetSelectedItems(std::vector<SessionItem*> items);

  /**
   * @brief Sets Qt selection flags.
   */
  void SetSelectionFlags(QItemSelectionModel::SelectionFlags flags);

signals:
  void SelectedItemChanged(mvvm::SessionItem*);

private:
  void OnViewModelReset();
  std::vector<SessionItem*> GetSelectedItemsIntern() const;
  void OnSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);

  std::unique_ptr<ViewModelDelegate> m_delegate;
  std::unique_ptr<ViewModel> m_view_model;
  QAbstractItemView* m_view{nullptr};
  QItemSelectionModel* m_selection_model{nullptr};
  std::vector<std::unique_ptr<QAbstractProxyModel>> m_proxy_chain;
  QItemSelectionModel::SelectionFlags m_selection_flags;
};

template <typename T>
T* ItemViewComponentProvider::GetSelected() const
{
  return dynamic_cast<T*>(GetSelectedItem());
}

template <typename T>
std::vector<T*> ItemViewComponentProvider::GetSelectedItems() const
{
  return utils::CastItems<T>(GetSelectedItemsIntern());
}

}  // namespace mvvm

#endif  // MVVM_PROVIDERS_ITEM_VIEW_COMPONENT_PROVIDER_H_
