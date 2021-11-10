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

#ifndef MVVM_VIEWMODEL_ABSTRACTVIEWMODELCONTROLLER_H
#define MVVM_VIEWMODEL_ABSTRACTVIEWMODELCONTROLLER_H

#include "mvvm/interfaces/modeleventlistenerinterface.h"
#include "mvvm/viewmodel_export.h"

#include <QStringList>

namespace ModelView
{
class SessionModel;
class ModelEventNotifierInterface;

//! Propagate changes

class MVVM_VIEWMODEL_EXPORT AbstractViewModelController : public ModelEventListenerInterface
{
public:
  virtual ~AbstractViewModelController() override;

  void SetNotifier(ModelEventNotifierInterface *notifier) override;

  void OnAboutToInsertItem(SessionItem *parent, const TagIndex &tag_index) override;

  void OnItemInserted(SessionItem *parent, const TagIndex &tag_index) override;

  void OnAboutToRemoveItem(SessionItem *parent, const TagIndex &tag_index) override;

  void OnItemRemoved(SessionItem *parent, const TagIndex &tag_index) override;

  void OnDataChanged(SessionItem *item, int role) override;

  void OnModelAboutToBeReset(SessionModel *model) override;

  void OnModelReset(SessionModel *model) override;

  void OnModelAboutToBeDestroyed(SessionModel* model) override;

  virtual void Init(SessionItem *root_item = nullptr);

  virtual QStringList GetHorizontalHeaderLabels() const;

protected:
  ModelEventNotifierInterface *m_notifier{nullptr};
};

}  // namespace ModelView

#endif  // MVVM_VIEWMODEL_ABSTRACTVIEWMODELCONTROLLER_H
