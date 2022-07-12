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

#ifndef TESTS_LIBTESTMACHINERY_MOCK_MODEL_H_
#define TESTS_LIBTESTMACHINERY_MOCK_MODEL_H_

#include <gmock/gmock.h>
#include <mvvm/interfaces/sessionmodel_interface.h>

#include <memory>

namespace mvvm
{
class SessionItem;
class TagIndex;
}  // namespace mvvm

//! Mocking class for SessionModelInterface.

class MockModel : public mvvm::SessionModelInterface
{
public:
  MOCK_METHOD(std::string, GetType, (), (const, override));
  MOCK_METHOD(mvvm::SessionItem*, GetRootItem, (), (const, override));

  MOCK_METHOD(mvvm::SessionItem*, InsertItem,
              (std::unique_ptr<mvvm::SessionItem> item, mvvm::SessionItem* parent,
               const mvvm::TagIndex& tag_index),
              (override));

  MOCK_METHOD(mvvm::SessionItem*, InsertNewItem,
              (const std::string& item_type, mvvm::SessionItem* parent,
               const mvvm::TagIndex& tag_index),
              (override));

  MOCK_METHOD(std::unique_ptr<mvvm::SessionItem>, TakeItem,
              (mvvm::SessionItem * parent, const mvvm::TagIndex& tag_index), (override));

  MOCK_METHOD(void, RemoveItem, (mvvm::SessionItem * item), (override));

  MOCK_METHOD(void, MoveItem,
              (mvvm::SessionItem * item, mvvm::SessionItem* new_parent,
               const mvvm::TagIndex& tag_index),
              (override));

  MOCK_METHOD(bool, SetData, (mvvm::SessionItem * item, const variant_t& value, int role),
              (override));

  MOCK_METHOD(void, Clear, (std::function<void(mvvm::SessionItem*)>), (override));
};

#endif  // TESTS_LIBTESTMACHINERY_MOCK_MODEL_LISTENER_H_
