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

#ifndef MVVM_VIEWS_ABSTRACT_ITEM_VIEW_H_
#define MVVM_VIEWS_ABSTRACT_ITEM_VIEW_H_

#include <mvvm/view_export.h>

#include <QWidget>

class QAbstractItemView;

namespace mvvm
{

class ISessionModel;
class SessionItem;
class ItemViewComponentProvider;

//! Generic view to show SessionModel in Qt lists, trees and tables.

class MVVM_VIEW_EXPORT AbstractItemView : public QWidget
{
  Q_OBJECT

public:
  explicit AbstractItemView(QWidget* parent_widget = nullptr);
  ~AbstractItemView() override;

  void SetComponentProvider(std::unique_ptr<ItemViewComponentProvider> provider);

  void SetApplicationModel(ISessionModel* model);

  void SetItem(SessionItem* item);

  ItemViewComponentProvider* GetComponentProvider();

  SessionItem* GetSelectedItem() const;
  void SetSelectedItem(SessionItem* item);

  template <typename T>
  T* GetSelected() const;

  std::vector<SessionItem*> GetSelectedItems() const;
  void SetSelectedItems(const std::vector<SessionItem*>& items);

signals:
  void SelectedItemChanged(mvvm::SessionItem*);

private:
  virtual void UpdateView();
  std::unique_ptr<ItemViewComponentProvider> m_provider;
};

template <typename T>
T* AbstractItemView::GetSelected() const
{
  return dynamic_cast<T*>(GetSelectedItem());
}

}  // namespace mvvm

#endif  // MVVM_VIEWS_ABSTRACT_ITEM_VIEW_H_
