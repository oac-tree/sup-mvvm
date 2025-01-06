/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_TEST_MOCK_ITEM_LISTENER_H_
#define MVVM_TEST_MOCK_ITEM_LISTENER_H_

#include <mvvm/signals/item_controller.h>

#include <gmock/gmock.h>

namespace mvvm
{
class SessionItem;
class TagIndex;
}  // namespace mvvm

namespace mvvm::test
{

/**
 * @brief The MockItemListener class is a mocking class to test events coming on item change.
 */
class MockItemListener : public mvvm::ItemListener
{
public:
  explicit MockItemListener(mvvm::SessionItem* item);

  MOCK_METHOD(void, OnItemInserted, (const mvvm::ItemInsertedEvent& event), ());
  MOCK_METHOD(void, OnAboutToRemoveItem, (const mvvm::AboutToRemoveItemEvent& event), ());
  MOCK_METHOD(void, OnItemRemoved, (const mvvm::ItemRemovedEvent& event), ());
  MOCK_METHOD(void, OnDataChanged, (const mvvm::DataChangedEvent& event), ());
  MOCK_METHOD(void, OnPropertyChanged, (const mvvm::PropertyChangedEvent& event), ());

  // we wrap mocking methods into other methods to be able to do additional testing in addition to
  // mocking

  void OnItemInsertedEvent(const mvvm::ItemInsertedEvent& event) { OnItemInserted(event); }
  void OnAboutToRemoveItemEvent(const mvvm::AboutToRemoveItemEvent& event)
  {
    OnAboutToRemoveItem(event);
  }
  void OnItemRemovedEvent(const mvvm::ItemRemovedEvent& event) { OnItemRemoved(event); }
  void OnDataChangedEvent(const mvvm::DataChangedEvent& event) { OnDataChanged(event); }
  void OnPropertyChangedEvent(const mvvm::PropertyChangedEvent& event) { OnPropertyChanged(event); }
};

}  // namespace mvvm::test

#endif  // MVVM_TEST_MOCK_ITEM_LISTENER_H_
