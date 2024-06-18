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

#ifndef MVVM_VIEWMODEL_VIEWMODEL_CONTROLLER_VIRTUAL_PARENT_IMPL_H_
#define MVVM_VIEWMODEL_VIEWMODEL_CONTROLLER_VIRTUAL_PARENT_IMPL_H_

#include <mvvm/model/path.h>
#include <mvvm/model/tagindex.h>
#include <mvvm/viewmodel/i_viewmodel_controller.h>
#include <mvvm/viewmodel/viewitem_map.h>

#include <memory>
#include <tuple>

namespace mvvm
{

class ISessionModel;
class ViewModelBase;
class IChildrenStrategy;
class IRowStrategy;

/**
 * @brief The ViewModelControllerImpl class contains implementation details for ViewModelController.
 *
 * @details This is a version which addresses virtual parentage issue.
 */

class ViewModelControllerVirtualParentImpl : public IViewModelController
{
public:
  explicit ViewModelControllerVirtualParentImpl(
      ViewModelBase *viewmodel, std::unique_ptr<IChildrenStrategy> children_strategy,
      std::unique_ptr<IRowStrategy> row_strategy);

  void OnModelEvent(const AboutToInsertItemEvent &event) override;

  void OnModelEvent(const ItemInsertedEvent &event) override;

  void OnModelEvent(const AboutToRemoveItemEvent &event) override;

  void OnModelEvent(const ItemRemovedEvent &event) override;

  void OnModelEvent(const DataChangedEvent &event) override;

  void OnModelEvent(const ModelResetEvent &event) override;

  void OnModelEvent(const ModelAboutToBeResetEvent &event) override;

  void OnModelEvent(const ModelAboutToBeDestroyedEvent &event) override;

  /**
   * @brief Returns current root item.
   */
  const SessionItem *GetRootItem() const override;

  /**
   * @brief Sets new root item.
   */
  void SetRootItem(SessionItem *root_item) override;

  int GetColumnCount() const override;

  QStringList GetHorizontalHeaderLabels() const override;

  void CheckInitialState() const;

  //! Returns an insert index for a view representing a child.
  //! Since number of views might not coincide with number of items (some items are marked)
  //! as hidden, we have to recalculate a view index.
  int GetInsertViewIndexOfChild(const SessionItem *parent, const SessionItem *child) const;

  //! Returns the parent view and the insert index for a child
  //! Walks up the tree until it finds a parent that wants to show the child
  std::tuple<ViewItem *, int> GetParentViewAndIndex(const SessionItem *item) const;

  /**
   * @brief Creates tree of rows with ViewItems representing given SessionItem and all its children.
   * @param item The item to explore.
   * @param is_root Item is treated as root item, when false.
   * @return Vector of ViewItem's containing other ViewItem's
   *
   * @details The method visits the item and all its children in non-recursive manner and creates a
   * tree of rows intended for ViewModel. The tree is formed basing on childred/row strategies.
   */
  std::vector<std::unique_ptr<ViewItem>> CreateTreeOfRows(SessionItem &item, bool is_root = false);

  ViewItemMap &GetViewItemMap();

private:
  ViewModelBase *m_viewmodel{nullptr};
  ViewItemMap m_view_item_map;
  std::unique_ptr<IChildrenStrategy> m_children_strategy;
  std::unique_ptr<IRowStrategy> m_row_strategy;
  Path m_root_item_path;  // saves path to custom root item, to restore it on model reset
};

}  // namespace mvvm

#endif  // MVVM_VIEWMODEL_VIEWMODEL_CONTROLLER_VIRTUAL_PARENT_IMPL_H_
