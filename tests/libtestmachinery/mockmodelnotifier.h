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
}  // namespace ModelView

//! Mocking class to test ModelEventNotifierInterface when it is called from ModelComposer.

class MockModelNotifier : public ModelView::ModelEventNotifierInterface
{
public:
  MOCK_METHOD1(EstablishConnections, void(ModelView::ModelEventListenerInterface* listener));

  MOCK_METHOD2(AboutToInsertItem,
               void(ModelView::SessionItem* parent, const ModelView::TagIndex& tag_index));

  MOCK_METHOD2(ItemInserted,
               void(ModelView::SessionItem* parent, const ModelView::TagIndex& tag_index));

  MOCK_METHOD2(AboutToRemoveItem,
               void(ModelView::SessionItem* parent, const ModelView::TagIndex& tag_index));

  MOCK_METHOD2(ItemRemoved,
               void(ModelView::SessionItem* parent, const ModelView::TagIndex& tag_index));

  MOCK_METHOD2(DataChanged, void(ModelView::SessionItem* item, int role));
};

#endif  // MOCKMODELNOTIFIER_H
