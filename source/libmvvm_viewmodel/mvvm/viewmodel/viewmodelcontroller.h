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

#ifndef MVVM_VIEWMODEL_VIEWMODECONTROLLER_H
#define MVVM_VIEWMODEL_VIEWMODECONTROLLER_H

#include "mvvm/viewmodel/abstractviewmodelcontroller.h"

#include <memory>

namespace ModelView
{
class SessionModel;
class ViewModelBase;
class ChildrenStrategyInterface;
class RowStrategyInterface;

//! Propagates changes from SessionModel to its ViewModelBase.
//!
class MVVM_VIEWMODEL_EXPORT ViewModelController : public AbstractViewModelController
{
public:
  ViewModelController(SessionModel *model, ViewModelBase *view_model);
  ~ViewModelController() override;

  void SetChildrenStrategy(std::unique_ptr<ChildrenStrategyInterface> children_strategy);

  void SetRowStrategy(std::unique_ptr<RowStrategyInterface> row_strategy);

  void OnItemInserted(SessionItem *parent, const TagIndex &tag_index) override;

  void OnAboutToRemoveItem(SessionItem *parent, const TagIndex &tag_index) override;

  void OnDataChanged(SessionItem *item, int role) override;

  void OnModelAboutToBeReset(SessionModel* model) override;

  void OnModelReset(SessionModel* model) override;

  void Init(SessionItem *root_item = nullptr) override;

  QStringList GetHorizontalHeaderLabels() const override;

private:
  struct ViewModelControllerImpl;
  std::unique_ptr<ViewModelControllerImpl> p_impl;
};

}  // namespace ModelView

#endif  // MVVM_VIEWMODEL_VIEWMODECONTROLLER_H
