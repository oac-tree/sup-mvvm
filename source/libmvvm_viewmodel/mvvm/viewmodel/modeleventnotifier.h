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

#ifndef MVVM_VIEWMODEL_MODELEVENTNOTIFIER_H
#define MVVM_VIEWMODEL_MODELEVENTNOTIFIER_H

#include "mvvm/interfaces/modeleventnotifierinterface.h"
#include "mvvm/viewmodel_export.h"

#include <QObject>
#include <map>
#include <vector>

namespace ModelView
{
//! Provides notification for all subscribers when some event happened with SessionModel.
//! Uses Qt's signal/slot machinery.
//! TODO consider home-made light-weighted signal/slot instead.

class MVVM_VIEWMODEL_EXPORT ModelEventNotifier : public QObject, public ModelEventNotifierInterface
{
  Q_OBJECT

public:
  ModelEventNotifier(ModelEventListenerInterface* listener = nullptr);
  ~ModelEventNotifier() override;

  void Unsubscribe(ModelEventListenerInterface* listener) override;

  void Subscribe(ModelEventListenerInterface* listener) override;

  void AboutToInsertItemNotify(SessionItem* parent, const TagIndex& tag_index) override;

  void ItemInsertedNotify(SessionItem* parent, const TagIndex& tag_index) override;

  void AboutToRemoveItemNotify(SessionItem* parent, const TagIndex& tag_index) override;

  void ItemRemovedNotify(SessionItem* parent, const TagIndex& tag_index) override;

  void DataChangedNotify(SessionItem* item, int role) override;

  void ModelAboutToBeResetNotify(SessionModel* model) override;

  void ModelResetNotify(SessionModel* model) override;

signals:
  void AboutToInsertItem(SessionItem* parent, const TagIndex& tag_index);

  void ItemInserted(SessionItem* parent, const TagIndex& tag_index);

  void AboutToRemoveItem(SessionItem* parent, const TagIndex& tag_index);

  void ItemRemoved(SessionItem* parent, const TagIndex& tag_index);

  void DataChanged(SessionItem* item, int role);

  void ModelAboutToBeReset(SessionModel* model);

  void ModelReset(SessionModel* model);

private:
  std::map<ModelEventListenerInterface*, std::vector<QMetaObject::Connection>> m_connections;
};

}  // namespace ModelView

#endif  // MVVM_VIEWMODEL_MODELEVENTNOTIFIER_H
