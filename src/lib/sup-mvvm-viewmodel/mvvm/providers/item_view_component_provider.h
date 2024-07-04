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

#ifndef MVVM_PROVIDERS_ITEM_VIEW_COMPONENT_PROVIDER_H_
#define MVVM_PROVIDERS_ITEM_VIEW_COMPONENT_PROVIDER_H_

#include <mvvm/utils/container_utils.h>

#include <QObject>
#include <memory>

class QAbstractItemView;
class QItemSelectionModel;

namespace mvvm
{

class SessionItem;
class ViewModel;
class ViewModelDelegate;
class ISessionModel;
class ItemSelectionModel;

/**
 * @brief The ItemViewComponentProvider class provides QAbstractItemView with custom components:
 * viewmodel, delegate and selection model.
 *
 * @details It owns all components, but doesn't own a view.
 */

class ItemViewComponentProvider : public QObject
{
  Q_OBJECT

public:
  ItemViewComponentProvider(std::unique_ptr<ViewModel> view_model, QAbstractItemView* view);
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
  QList<QModelIndex> GetViewIndices(const mvvm::SessionItem* item) const;

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

signals:
  void SelectedItemChanged(mvvm::SessionItem*);

private:
  std::vector<SessionItem*> GetSelectedItemsIntern() const;

  std::unique_ptr<ViewModelDelegate> m_delegate;
  std::unique_ptr<ItemSelectionModel> m_selection_model;
  std::unique_ptr<ViewModel> m_view_model;
  QAbstractItemView* m_view{nullptr};
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

/**
 * @brief Convenience function to create component provider for a view.
 *
 * @tparam ViewModelT The type of ViewModel which will be created.
 * @param view The view which will be served by the provider.
 * @param model The model which will be used to setup viewmodel.
 *
 * @return Component provider.
 */
template <typename ViewModelT>
std::unique_ptr<ItemViewComponentProvider> CreateProvider(QAbstractItemView* view,
                                                          ISessionModel* model = nullptr)
{
  auto result =
      std::make_unique<ItemViewComponentProvider>(std::make_unique<ViewModelT>(model), view);
  result->SetApplicationModel(model);
  return result;
}

}  // namespace mvvm

#endif  // MVVM_PROVIDERS_ITEM_VIEW_COMPONENT_PROVIDER_H_
