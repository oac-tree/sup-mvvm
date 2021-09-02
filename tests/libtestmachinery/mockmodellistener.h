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

#ifndef MOCKMODELLISTENER_H
#define MOCKMODELLISTENER_H

#include "mvvm/interfaces/modeleventlistenerinterface.h"

#include <gmock/gmock.h>

namespace ModelView
{
class SessionItem;
class TagIndex;
}  // namespace ModelView

//! Mocking class to test ModelEventListenerInterface reactions on notifications issued by
//! ModelEventNotifier.

class MockModelListener : public ModelView::ModelEventListenerInterface
{
public:
  MOCK_METHOD2(OnAboutToInsertItem,
               void(ModelView::SessionItem* parent, const ModelView::TagIndex& tag_index));

  MOCK_METHOD2(OnItemInserted,
               void(ModelView::SessionItem* parent, const ModelView::TagIndex& tag_index));

  MOCK_METHOD2(OnAboutToRemoveItem,
               void(ModelView::SessionItem* parent, const ModelView::TagIndex& tag_index));

  MOCK_METHOD2(OnItemRemoved,
               void(ModelView::SessionItem* parent, const ModelView::TagIndex& tag_index));

  MOCK_METHOD2(OnDataChanged, void(ModelView::SessionItem* item, int role));
};

#endif  // MOCKMODELLISTENER_H
