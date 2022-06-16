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

#ifndef MVVM_WIDGETS_ITEMVIEWCONTROLLER_H
#define MVVM_WIDGETS_ITEMVIEWCONTROLLER_H

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
class ApplicationModel;

//! Provides QAbstractItemView with custom components: viewmodel, delegate
//! and selection model. It owns all components, but doesn't own a view.

class ItemViewComponentProvider : public QObject
{
  Q_OBJECT

public:
  using create_viewmodel_t =
      std::function<std::unique_ptr<mvvm::ViewModel>(mvvm::ApplicationModel*)>;

  ItemViewComponentProvider(create_viewmodel_t model_func, QAbstractItemView* view);
  ~ItemViewComponentProvider() override;

  void SetApplicationModel(mvvm::ApplicationModel* model);

  void SetItem(mvvm::SessionItem* item);

  QAbstractItemView* GetView() const;

  mvvm::ItemSelectionModel* GetSelectionModel() const;

  mvvm::ViewModel* GetViewModel() const;

  mvvm::SessionItem* GetSelectedItem() const;
  void SetSelectedItem(mvvm::SessionItem* item);

  template <typename T>
  T* GetSelected() const;

  template <typename T = mvvm::SessionItem>
  std::vector<T*> GetSelectedItems() const;

  void SetSelectedItems(std::vector<mvvm::SessionItem*> items);

signals:
  void SelectedItemChanged(mvvm::SessionItem*);

private:
  void Reset();
  void InitViewModel(mvvm::ApplicationModel* model);
  std::vector<mvvm::SessionItem*> GetSelectedItemsIntern() const;

  std::unique_ptr<mvvm::ViewModelDelegate> m_delegate;
  std::unique_ptr<mvvm::ItemSelectionModel> m_selection_model;
  std::unique_ptr<mvvm::ViewModel> m_view_model;
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
  return ::mvvm::utils::CastItems<T>(GetSelectedItemsIntern());
}

template <typename T>
std::unique_ptr<T> CreateViewModel(ApplicationModel* model)
{
  return std::make_unique<T>(model);
}

template <typename ViewModelT>
std::unique_ptr<ItemViewComponentProvider> CreateProvider(QAbstractItemView* view,
                                                          ApplicationModel* model = nullptr)
{
  auto result = std::make_unique<ItemViewComponentProvider>(CreateViewModel<ViewModelT>, view);
  result->SetApplicationModel(model);
  return result;
}

}  // namespace mvvm

#endif  // MVVM_WIDGETS_ITEMVIEWCONTROLLER_H
