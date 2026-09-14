/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#ifndef MVVM_VIEWS_ITEM_VIEW_COMPONENT_PROVIDER_H_
#define MVVM_VIEWS_ITEM_VIEW_COMPONENT_PROVIDER_H_

#include <QItemSelectionModel>
#include <QObject>
#include <memory>

class QAbstractItemView;
class QAbstractProxyModel;
class QAbstractItemDelegate;

namespace mvvm
{

class SessionItem;
class ViewModel;
class ISessionModel;
class ItemSelection;

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
  ItemViewComponentProvider(std::unique_ptr<QAbstractItemDelegate> view_model_delegate,
                            std::unique_ptr<ViewModel> view_model, QAbstractItemView* view);

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
  mvvm::SessionItem* GetItemFromViewIndex(const QModelIndex& index) const;

  /**
   * @brief Returns view indices for given item.
   *
   * View indices can be used for operation through a view. The method takes into account
   * the existence of proxy model. When proxy model is set, the result is obtained by applying
   * QAbstractItemProxyModel::mapFromSource to the original item's indices.
   */
  QList<QModelIndex> GetViewIndexes(const mvvm::SessionItem* item) const;

  /**
   * @brief Returns current selection as an object.
   */
  ItemSelection GetSelection() const;

  /**
   * @brief Makes all specified items selected in a view.
   */
  void SetSelection(const ItemSelection& selection);

  /**
   * @brief Returns selection flags.
   */
  QItemSelectionModel::SelectionFlags GetSelectionFlags() const;

  /**
   * @brief Sets Qt selection flags.
   */
  void SetSelectionFlags(QItemSelectionModel::SelectionFlags flags);

signals:
  void SelectionChanged(const mvvm::ItemSelection&);

private:
  void OnViewModelReset() const;
  void OnSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);

  std::unique_ptr<QAbstractItemDelegate> m_delegate;
  std::unique_ptr<ViewModel> m_view_model;
  QAbstractItemView* m_view{nullptr};
  QItemSelectionModel* m_selection_model{nullptr};
  std::vector<std::unique_ptr<QAbstractProxyModel>> m_proxy_chain;
  QItemSelectionModel::SelectionFlags m_selection_flags;
};

}  // namespace mvvm

#endif  // MVVM_VIEWS_ITEM_VIEW_COMPONENT_PROVIDER_H_
