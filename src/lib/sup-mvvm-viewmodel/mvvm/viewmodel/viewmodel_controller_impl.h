/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_VIEWMODEL_VIEWMODEL_CONTROLLER_IMPL_H_
#define MVVM_VIEWMODEL_VIEWMODEL_CONTROLLER_IMPL_H_

#include <mvvm/model/path.h>
#include <mvvm/model/tagindex.h>
#include <mvvm/signals/event_types.h>
#include <mvvm/viewmodel/viewitem_map.h>

#include <QStringList>
#include <memory>

namespace mvvm
{

class SessionModelInterface;
class ViewModelBase;
class ChildrenStrategyInterface;
class RowStrategyInterface;

/**
 * @brief The ViewModelControllerImpl class contains implementation details for ViewModelController.
 */

class ViewModelControllerImpl
{
public:
  ViewModelControllerImpl(SessionModelInterface *model, ViewModelBase *view_model);

  void SetChildrenStrategy(std::unique_ptr<ChildrenStrategyInterface> children_strategy);

  void SetRowStrategy(std::unique_ptr<RowStrategyInterface> row_strategy);

  void OnModelEvent(const ItemInsertedEvent &event);

  void OnModelEvent(const AboutToRemoveItemEvent &event);

  void OnModelEvent(const DataChangedEvent &event);

  void OnModelEvent(const ModelResetEvent &event);

  void OnModelEvent(const ModelAboutToBeResetEvent &event);

  void OnModelEvent(const ModelAboutToBeDestroyedEvent &event);

  void Init(SessionItem *custom_root_item);

  QStringList GetHorizontalHeaderLabels() const;

  void CheckInitialState() const;

  const SessionItem *GetRootItem() const;

  //! Returns an insert index for a view representing a child.
  //! Since number of views might not coincide with number of items (some items are marked)
  //! as hidden, we have to recalculate a view index.
  int GetInsertViewIndexOfChild(const SessionItem *parent, const SessionItem *child);

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

  bool isValidItemRole(const mvvm::ViewItem *view, int item_role);

private:
  SessionModelInterface *m_model{nullptr};
  ViewModelBase *m_view_model{nullptr};
  ViewItemMap m_view_item_map;
  std::unique_ptr<ChildrenStrategyInterface> m_children_strategy;
  std::unique_ptr<RowStrategyInterface> m_row_strategy;
  Path m_root_item_path;  // saves path to custom root item, to restore it on model reset
};

}  // namespace mvvm

#endif  // MVVM_VIEWMODEL_VIEWMODEL_CONTROLLER_H_
