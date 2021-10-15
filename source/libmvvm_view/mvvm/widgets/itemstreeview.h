/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
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

#include <QWidget>
#include <memory>

class QTreeView;
class QItemSelection;
class QItemSelectionModel;

namespace ModelView
{
class SessionItem;
class ViewModel;

//! Tree view to show items of SessionModel via ViewModel mechanism.
//! Provides notification mechanism for SessionItem selections, use custom delegate.

class MVVM_VIEW_EXPORT ItemsTreeView : public QWidget
{
  Q_OBJECT

public:
  explicit ItemsTreeView(QWidget* parent = nullptr);
  ~ItemsTreeView() override;

  QTreeView* GetTreeView();

  void SetViewModel(std::unique_ptr<ViewModel> viewModel);

  // FIXME restore delegates
  //  void setViewModelDelegate(std::unique_ptr<ViewModelDelegate> delegate);

  void SetSelected(SessionItem* item);

  void SetRootSessionItem(SessionItem* item);

  ViewModel* GetViewModel() const;

signals:
  void itemSelected(const SessionItem*);

private slots:
  void onSelectionChanged(const QItemSelection&, const QItemSelection&);

private:
  QItemSelectionModel* GetSelectionModel();

  void SetConnected(bool flag);

  QTreeView* m_treeView{nullptr};
  std::unique_ptr<ViewModel> m_viewModel;
  bool m_block_selection;
};

}  // namespace ModelView

#endif  // MVVM_WIDGETS_ITEMSTREEVIEW_H
