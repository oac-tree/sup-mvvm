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

#ifndef MOCKITEMLISTENER
#define MOCKITEMLISTENER

#include "mvvm/signals/itemlistener.h"

#include <gmock/gmock.h>

#include <memory>

namespace mvvm
{
class SessionItem;
class TagIndex;
}  // namespace mvvm

//! Mocking class to test ModelEventListenerInterface reactions on notifications issued by
//! ModelEventNotifier.

class MockItemListener : public mvvm::ItemListener<mvvm::SessionItem>
{
public:
  explicit MockItemListener(mvvm::SessionItem* item) { SetItem(item); }

  MOCK_METHOD2(OnItemInserted, void(mvvm::SessionItem* item, const mvvm::TagIndex& tagindex));
  MOCK_METHOD2(OnAboutToRemoveItem, void(mvvm::SessionItem* item, const mvvm::TagIndex& tagindex));
  MOCK_METHOD2(OnItemRemoved, void(mvvm::SessionItem* item, const mvvm::TagIndex& tagindex));
  MOCK_METHOD2(OnDataChanged, void(mvvm::SessionItem* item, int role));
  MOCK_METHOD2(OnPropertyChanged, void(mvvm::SessionItem* item, std::string name));

  MOCK_METHOD0(Unsubscribe, void(void));

protected:
  void Subscribe() override;
};

#endif  // MOCKMODELLISTENER_H
