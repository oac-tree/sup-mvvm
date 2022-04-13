/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_WIDGETS_ITEMSTREEVIEW_H
#define MVVM_WIDGETS_ITEMSTREEVIEW_H

#include "mvvm/view_export.h"

#include "mvvm/model/sessionitem.h"

#include <QWidget>
#include <memory>

class QTreeView;
class QItemSelection;
class QItemSelectionModel;

namespace mvvm
{
class SessionItem;
class ViewModel;
class ViewModelDelegate;

//! Tree view to show items of SessionModel via ViewModel mechanism.
//! Provides notification mechanism for SessionItem selections, use custom delegate.

class MVVM_VIEW_EXPORT ItemsTreeView : public QWidget
{
  Q_OBJECT

public:
  explicit ItemsTreeView(QWidget* parent = nullptr);
  ~ItemsTreeView() override;

  QTreeView* GetTreeView();

  void Reset();

  void SetViewModel(std::unique_ptr<ViewModel> view_model);

  void SetViewModelDelegate(std::unique_ptr<ViewModelDelegate> delegate);

  void SetSelectedItem(SessionItem* item);

  void SetSelectedItems(const std::vector<SessionItem*>& to_select);

  virtual void SetRootSessionItem(SessionItem* item);

  ViewModel* GetViewModel() const;

  //! Returns first selected item, or nullptr if nothing is selected.
  SessionItem* GetSelectedItem() const;

  //! Returns all selected items.
  std::vector<SessionItem*> GetSelectedItems() const;

signals:
  void itemSelected(mvvm::SessionItem*);

private:
  void onSelectionChanged(const QItemSelection&, const QItemSelection&);

  QItemSelectionModel* GetSelectionModel();

  void SetConnected(bool flag);

  QTreeView* m_tree_view{nullptr};
  std::unique_ptr<ViewModel> m_view_model;
  std::unique_ptr<ViewModelDelegate> m_delegate;
  bool m_block_selection;
};

}  // namespace mvvm

Q_DECLARE_METATYPE(mvvm::SessionItem*)

#endif  // MVVM_WIDGETS_ITEMSTREEVIEW_H
