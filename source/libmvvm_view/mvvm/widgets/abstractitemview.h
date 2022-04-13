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

#ifndef MVVM_WIDGETS_ABSTRACTITEMVIEW_H
#define MVVM_WIDGETS_ABSTRACTITEMVIEW_H

#include "mvvm/widgets/itemstreeview.h"

class QAbstractItemView;

namespace mvvm
{

class SessionItem;
class ViewModel;
class ViewModelDelegate;
class ItemSelectionModel;

//! Generic view to show SessionModel in Qt lists, trees and tables.

class MVVM_VIEW_EXPORT AbstractItemView : public QWidget
{
  Q_OBJECT

public:
  explicit AbstractItemView(QWidget* parent = nullptr);
  ~AbstractItemView() override;

  virtual void SetItem(SessionItem* item);

  QAbstractItemView* GetView() const;

  void SetView(QAbstractItemView* view);

  void SetViewModel(std::unique_ptr<ViewModel> view_model);

  ViewModel* GetViewModel() const;

  const mvvm::SessionItem* GetSelectedItem() const;

  std::vector<const mvvm::SessionItem*> GetSelectedItems() const;

  void SetSelectedItem(const mvvm::SessionItem* item);

  void SetSelectedItems(std::vector<const mvvm::SessionItem*> items);

  void Reset();

signals:
  void SelectedItemChanged(const mvvm::SessionItem*);

private:
  QAbstractItemView* m_view{nullptr};
  std::unique_ptr<ViewModelDelegate> m_delegate;
  std::unique_ptr<ItemSelectionModel> m_selection_model;
  std::unique_ptr<ViewModel> m_view_model;
};

}  // namespace mvvm

#endif  // MVVM_WIDGETS_ABSTRACTITEMVIEW_H
