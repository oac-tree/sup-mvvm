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

#ifndef MVVM_PROVIDERS_VIEWMODEL_CONTROLLER_IMPL_H_
#define MVVM_PROVIDERS_VIEWMODEL_CONTROLLER_IMPL_H_

#include <mvvm/providers/i_viewmodel_controller.h>
#include <mvvm/providers/item_view_map.h>
#include <mvvm/signals/event_types.h>

#include <QStringList>
#include <memory>

namespace mvvm
{

class ISessionModel;
class ViewModelBase;
class IChildrenStrategy;
class IRowStrategy;
class ViewItem;

/**
 * @brief The ViewModelControllerImpl class contains implementation details for ViewModelController.
 */
class ViewModelControllerImpl : public IViewModelController
{
public:
  using item_view_map_t = ItemViewMap<const SessionItem *, ViewItem *>;
  explicit ViewModelControllerImpl(ViewModelBase *viewmodel,
                                   std::unique_ptr<IChildrenStrategy> children_strategy,
                                   std::unique_ptr<IRowStrategy> row_strategy);

  ~ViewModelControllerImpl() override;

  ViewModelControllerImpl(const ViewModelControllerImpl &other) = delete;
  ViewModelControllerImpl &operator=(const ViewModelControllerImpl &other) = delete;
  ViewModelControllerImpl(ViewModelControllerImpl &&other) = delete;
  ViewModelControllerImpl &operator=(ViewModelControllerImpl &&) = delete;

  void OnAboutToInsertItemEvent(const AboutToInsertItemEvent &event) override;

  void OnItemInsertedEvent(const ItemInsertedEvent &event) override;

  void OnItemRemovedEvent(const ItemRemovedEvent &event) override;

  void OnAboutToRemoveItemEvent(const AboutToRemoveItemEvent &event) override;

  void OnDataChangedEvent(const DataChangedEvent &event) override;

  void OnModelResetEvent(const ModelResetEvent &event) override;

  void OnModelAboutToBeResetEvent(const ModelAboutToBeResetEvent &event) override;

  void OnModelAboutToBeDestroyedEvent(const ModelAboutToBeDestroyedEvent &event) override;

  const SessionItem *GetRootItem() const override;

  void SetRootItem(SessionItem *root_item) override;

  std::size_t GetColumnCount() const override;

  QStringList GetHorizontalHeaderLabels() const override;

  void CheckInitialState() const;

  /**
   * @brief Returns an insert index for a view representing a child.
   *
   * Since number of views might not coincide with number of items (some items are marked) as
   * hidden, we have to recalculate a view index.
   */
  std::size_t GetInsertViewIndexOfChild(const SessionItem *parent, const SessionItem *child);

  /**
   * @brief Creates tree of rows with ViewItems representing given SessionItem and all its children.
   *
   * The method visits the item and all its children in non-recursive manner and creates a tree of
   * rows intended for ViewModel. The tree is formed basing on childred/row strategies.
   *
   * @param item The item to explore.
   * @param is_root Item is treated as root item, when false.
   * @return Vector of ViewItem's containing other ViewItem's
   */
  std::vector<std::unique_ptr<ViewItem>> CreateTreeOfRows(SessionItem &item, bool is_root = false);

  item_view_map_t &GetViewItemMap();

private:
  ViewModelBase *m_view_model{nullptr};
  item_view_map_t m_view_item_map;
  std::unique_ptr<IChildrenStrategy> m_children_strategy;
  std::unique_ptr<IRowStrategy> m_row_strategy;
};

}  // namespace mvvm

#endif  // MVVM_PROVIDERS_VIEWMODEL_CONTROLLER_IMPL_H_
