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

#ifndef MOCKMODELNOTIFIER_H
#define MOCKMODELNOTIFIER_H

#include "mvvm/interfaces/modeleventnotifierinterface.h"

#include <gmock/gmock.h>

namespace ModelView
{
class SessionItem;
class TagIndex;
class ModelEventListenerInterface;
class SessionModel;
}  // namespace ModelView

//! Mocking class to test ModelEventNotifierInterface when it is called from ModelComposer.

class MockModelNotifier : public ModelView::ModelEventNotifierInterface
{
public:
  MOCK_METHOD1(Unsubscribe, void(ModelView::ModelEventListenerInterface* listener));

  MOCK_METHOD1(Subscribe, void(ModelView::ModelEventListenerInterface* listener));

  MOCK_METHOD2(AboutToInsertItemNotify,
               void(ModelView::SessionItem* parent, const ModelView::TagIndex& tag_index));

  MOCK_METHOD2(ItemInsertedNotify,
               void(ModelView::SessionItem* parent, const ModelView::TagIndex& tag_index));

  MOCK_METHOD2(AboutToRemoveItemNotify,
               void(ModelView::SessionItem* parent, const ModelView::TagIndex& tag_index));

  MOCK_METHOD2(ItemRemovedNotify,
               void(ModelView::SessionItem* parent, const ModelView::TagIndex& tag_index));

  MOCK_METHOD2(DataChangedNotify, void(ModelView::SessionItem* item, int role));

  MOCK_METHOD1(ModelAboutToBeResetNotify, void(ModelView::SessionModel* model));

  MOCK_METHOD1(ModelResetNotify, void(ModelView::SessionModel* model));

  MOCK_METHOD1(ModelAboutToBeDestroyedNotify, void(ModelView::SessionModel* model));
};

#endif  // MOCKMODELNOTIFIER_H
