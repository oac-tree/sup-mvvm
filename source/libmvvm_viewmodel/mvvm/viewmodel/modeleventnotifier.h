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

namespace ModelView
{
//! Provides notification for all subscribers when some event happened with SessionModel.
//! Uses Qt's signal/slot machinery.
//! TODO consider home-made light-weighted signal/slot instead.

class MVVM_VIEWMODEL_EXPORT ModelEventNotifier : public QObject, public ModelEventNotifierInterface
{
  Q_OBJECT

public:
  ModelEventNotifier() = default;
  ModelEventNotifier(ModelEventListenerInterface* listener);

  void EstablishConnections(ModelEventListenerInterface* listener) override;

  void AboutToInsertItem(SessionItem* parent, const TagIndex& tag_index) override;

  void ItemInserted(SessionItem* parent, const TagIndex& tag_index) override;

  void AboutToRemoveItem(SessionItem* parent, const TagIndex& tag_index) override;

  void ItemRemoved(SessionItem* parent, const TagIndex& tag_index) override;

  void DataChanged(SessionItem* item, int role) override;

signals:
  void InvokeAboutToInsertItem(SessionItem* parent, const TagIndex& tag_index);

  void InvokeItemInserted(SessionItem* parent, const TagIndex& tag_index);

  void InvokeAboutToRemoveItem(SessionItem* parent, const TagIndex& tag_index);

  void InvokeItemRemoved(SessionItem* parent, const TagIndex& tag_index);

  void InvokeDataChanged(SessionItem* item, int role);
};

}  // namespace ModelView

#endif  // MVVM_VIEWMODEL_MODELEVENTNOTIFIER_H
