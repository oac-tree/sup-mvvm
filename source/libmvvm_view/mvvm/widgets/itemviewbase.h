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

#ifndef MVVM_WIDGETS_ITEMVIEWBASE_H
#define MVVM_WIDGETS_ITEMVIEWBASE_H

#include "mvvm/model/sessionitem.h"
#include "mvvm/view_export.h"

#include <QWidget>
#include <memory>

class QAbstractItemView;

namespace mvvm
{

class SessionItem;
class ViewModel;
class ViewModelDelegate;
class ItemSelectionModel;

//! Generic view to show SessionModel in Qt lists, trees and tables.

class MVVM_VIEW_EXPORT ItemViewBase : public QWidget
{
  Q_OBJECT

public:
  explicit ItemViewBase(QWidget* parent = nullptr);
  ~ItemViewBase() override;

  void SetRootSessionItem(SessionItem* item);

  QAbstractItemView* GetView() const;

  void SetView(QAbstractItemView* view);

  void SetViewModel(std::unique_ptr<ViewModel> view_model);

  ViewModel* GetViewModel() const;

  mvvm::SessionItem* GetSelectedItem() const;

  void SetSelectedItem(mvvm::SessionItem* item);

  void SetSelectedItems(std::vector<mvvm::SessionItem*> items);

  void Reset();

  template <typename T>
  T* GetSelected() const;

  template <typename T = SessionItem>
  std::vector<T*> GetSelectedItems() const;

signals:
  void SelectedItemChanged(mvvm::SessionItem*);

private:
  std::vector<mvvm::SessionItem*> GetSelectedItemsIntern() const;

  QAbstractItemView* m_view{nullptr};
  std::unique_ptr<ViewModelDelegate> m_delegate;
  std::unique_ptr<ItemSelectionModel> m_selection_model;
  std::unique_ptr<ViewModel> m_view_model;
};

template <typename T>
T* ItemViewBase::GetSelected() const
{
  return dynamic_cast<T*>(GetSelectedItem());
}

template <typename T>
std::vector<T*> ItemViewBase::GetSelectedItems() const
{
  return ::mvvm::utils::CastItems<T>(GetSelectedItemsIntern());
}

}  // namespace mvvm

#endif  // MVVM_WIDGETS_ITEMVIEWBASE_H
