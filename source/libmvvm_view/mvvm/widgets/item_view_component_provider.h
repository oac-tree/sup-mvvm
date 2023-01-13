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

#ifndef MVVM_WIDGETS_ITEM_VIEW_COMPONENT_PROVIDER_H_
#define MVVM_WIDGETS_ITEM_VIEW_COMPONENT_PROVIDER_H_

#include <mvvm/utils/container_utils.h>

#include <QObject>
#include <functional>
#include <memory>

class QAbstractItemView;

namespace mvvm
{

class SessionItem;
class ItemSelectionModel;
class ViewModel;
class ViewModelDelegate;
class SessionModelInterface;

//! Provides QAbstractItemView with custom components: viewmodel, delegate
//! and selection model. It owns all components, but doesn't own a view.

class ItemViewComponentProvider : public QObject
{
  Q_OBJECT

public:
  using create_viewmodel_t = std::function<std::unique_ptr<ViewModel>(SessionModelInterface*)>;

  ItemViewComponentProvider(create_viewmodel_t model_func, QAbstractItemView* view);
  ~ItemViewComponentProvider() override;

  void SetApplicationModel(SessionModelInterface* model);

  void SetItem(SessionItem* item);

  QAbstractItemView* GetView() const;

  ItemSelectionModel* GetSelectionModel() const;

  ViewModel* GetViewModel() const;

  SessionItem* GetSelectedItem() const;
  void SetSelectedItem(SessionItem* item);

  template <typename T>
  T* GetSelected() const;

  template <typename T = SessionItem>
  std::vector<T*> GetSelectedItems() const;

  void SetSelectedItems(std::vector<SessionItem*> items);

signals:
  void SelectedItemChanged(mvvm::SessionItem*);

private:
  void Reset();
  void InitViewModel(SessionModelInterface* model);
  std::vector<SessionItem*> GetSelectedItemsIntern() const;

  std::unique_ptr<ViewModelDelegate> m_delegate;
  std::unique_ptr<ItemSelectionModel> m_selection_model;
  std::unique_ptr<ViewModel> m_view_model;
  create_viewmodel_t m_create_viewmodel;  //!< to create new ViewModel on ApplicationModel change

  QAbstractItemView* m_view{nullptr};
};

template <typename T>
T* ItemViewComponentProvider::GetSelected() const
{
  return dynamic_cast<T*>(GetSelectedItem());
}

template <typename T>
std::vector<T*> ItemViewComponentProvider::GetSelectedItems() const
{
  return utils::CastItems<T>(GetSelectedItemsIntern());
}

template <typename T>
std::unique_ptr<T> CreateViewModel(SessionModelInterface* model)
{
  return std::make_unique<T>(model);
}

template <typename ViewModelT>
std::unique_ptr<ItemViewComponentProvider> CreateProvider(QAbstractItemView* view,
                                                          SessionModelInterface* model = nullptr)
{
  auto result = std::make_unique<ItemViewComponentProvider>(CreateViewModel<ViewModelT>, view);
  result->SetApplicationModel(model);
  return result;
}

}  // namespace mvvm

#endif  // MVVM_WIDGETS_ITEM_VIEW_COMPONENT_PROVIDER_H_
