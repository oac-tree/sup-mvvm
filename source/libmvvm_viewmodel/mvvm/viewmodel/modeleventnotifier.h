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

#include <map>
#include <memory>
#include <vector>

namespace mvvm
{
//! Provides notification for all subscribers when some event happened with SessionModel.

class MVVM_VIEWMODEL_EXPORT ModelEventNotifier : public ModelEventNotifierInterface
{
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

  void ModelAboutToBeDestroyedNotify(SessionModel* model) override;

private:
  struct ModelEventNotifierImpl;
  std::unique_ptr<ModelEventNotifierImpl> p_impl;
};

}  // namespace mvvm

#endif  // MVVM_VIEWMODEL_MODELEVENTNOTIFIER_H
