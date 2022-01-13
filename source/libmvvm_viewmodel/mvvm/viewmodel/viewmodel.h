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

#ifndef MVVM_VIEWMODEL_VIEWMODEL_H
#define MVVM_VIEWMODEL_VIEWMODEL_H

#include "mvvm/viewmodelbase/viewmodelbase.h"

namespace mvvm
{
class SessionModel;
class SessionItem;
class ViewItem;
class AbstractViewModelController;

//! Main class to represent content of SessionModel in Qt's trees and tables.

class MVVM_VIEWMODEL_EXPORT ViewModel : public ViewModelBase
{
  Q_OBJECT

public:
  ViewModel(QObject* parent = nullptr);
  ~ViewModel() override;

  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;

  const SessionItem* GetRootSessionItem() const;
  SessionItem* GetRootSessionItem();

  void SetRootSessionItem(SessionItem* item);

  const SessionItem* GetSessionItemFromIndex(const QModelIndex& index) const;

  ViewItem* GetViewItemFromIndex(const QModelIndex& index) const;

  std::vector<ViewItem*> FindViews(const mvvm::SessionItem* item) const;

  QModelIndexList GetIndexOfSessionItem(const SessionItem* item) const;

protected:
  void SetController(std::unique_ptr<AbstractViewModelController> controller);
  AbstractViewModelController* Controller();

private:
  std::unique_ptr<AbstractViewModelController> m_controller;
};

}  // namespace mvvm

#endif  // MVVM_VIEWMODEL_VIEWMODEL_H
