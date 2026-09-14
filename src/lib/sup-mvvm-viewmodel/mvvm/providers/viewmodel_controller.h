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

#ifndef MVVM_PROVIDERS_VIEWMODEL_CONTROLLER_H_
#define MVVM_PROVIDERS_VIEWMODEL_CONTROLLER_H_

#include <mvvm/providers/abstract_viewmodel_controller.h>
#include <mvvm/providers/item_view_map.h>

#include <memory>
#include <vector>

namespace mvvm
{

class ViewModelBase;
class IChildrenStrategy;
class IRowStrategy;
class ViewItem;

/**
 * @brief The ViewModelController class propagates changes from SessionModel to ViewModel.
 *
 * It listens to the SessionModel and updates the ViewModel accordingly, using pluggable
 * children/row strategies to build the tree of views. To customize the behavior, derive from this
 * class and override the relevant OnXxxEvent handlers; protected helpers and members give access to
 * the underlying machinery (view item map, strategies, view model).
 */
class ViewModelController : public AbstractViewModelController
{
public:
  using item_view_map_t = ItemViewMap<const SessionItem*, ViewItem*>;

  ViewModelController(ViewModelBase* viewmodel,
                      std::unique_ptr<IChildrenStrategy> children_strategy,
                      std::unique_ptr<IRowStrategy> row_strategy);

  ~ViewModelController() override;

  ViewModelController(const ViewModelController& other) = delete;
  ViewModelController& operator=(const ViewModelController& other) = delete;
  ViewModelController(ViewModelController&& other) = delete;
  ViewModelController& operator=(ViewModelController&&) = delete;

  void OnItemInsertedEvent(const ItemInsertedEvent& event) override;

  void OnAboutToRemoveItemEvent(const AboutToRemoveItemEvent& event) override;

  void OnDataChangedEvent(const DataChangedEvent& event) override;

  void OnModelAboutToBeResetEvent(const ModelAboutToBeResetEvent& event) override;

  void OnModelResetEvent(const ModelResetEvent& event) override;

  void OnModelAboutToBeDestroyedEvent(const ModelAboutToBeDestroyedEvent& event) override;

  const SessionItem* GetRootItem() const override;

  std::size_t GetColumnCount() const override;

  QStringList GetHorizontalHeaderLabels() const override;

protected:
  /**
   * @brief Returns an insert index for a view representing a child.
   *
   * Since number of views might not coincide with number of items (some items are marked) as
   * hidden, we have to recalculate a view index.
   */
  std::size_t GetInsertViewIndexOfChild(SessionItem* parent, const SessionItem* child);

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
  std::vector<std::unique_ptr<ViewItem>> CreateTreeOfRows(SessionItem& item, bool is_root = false);

  item_view_map_t& GetViewItemMap() &;

  ViewModelBase* m_view_model{nullptr};
  item_view_map_t m_view_item_map;
  std::unique_ptr<IChildrenStrategy> m_children_strategy;
  std::unique_ptr<IRowStrategy> m_row_strategy;

private:
  void SetRootItemImpl(SessionItem* root_item) override;
};

}  // namespace mvvm

#endif  // MVVM_PROVIDERS_VIEWMODEL_CONTROLLER_H_
