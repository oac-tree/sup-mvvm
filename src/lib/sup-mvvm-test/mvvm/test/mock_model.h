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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_TEST_MOCK_MODEL_H_
#define MVVM_TEST_MOCK_MODEL_H_

#include <mvvm/model/i_session_model.h>
#include <mvvm/model/session_item.h>

#include <gmock/gmock.h>

namespace mvvm::test
{

/**
 * @brief Mocking class for ISessionModel.
 */
class MockModel : public mvvm::ISessionModel
{
public:
  MOCK_METHOD(std::string, GetType, (), (const, override));

  MOCK_METHOD(mvvm::SessionItem *, GetRootItem, (), (const, override));

  MOCK_METHOD(mvvm::ModelEventHandler *, GetEventHandler, (), (const, override));

  MOCK_METHOD(mvvm::ICommandStack *, GetCommandStack, (), (const, override));

  MOCK_METHOD(mvvm::SessionItem *, InsertItem,
              (std::unique_ptr<mvvm::SessionItem> item, mvvm::SessionItem *parent,
               const mvvm::TagIndex &tag_index),
              (override));

  MOCK_METHOD(std::unique_ptr<mvvm::SessionItem>, TakeItem,
              (mvvm::SessionItem * parent, const mvvm::TagIndex &tag_index), (override));

  MOCK_METHOD(void, RemoveItem, (mvvm::SessionItem * item), (override));

  MOCK_METHOD(void, MoveItem,
              (mvvm::SessionItem * item, mvvm::SessionItem *new_parent,
               const mvvm::TagIndex &tag_index),
              (override));

  MOCK_METHOD(bool, SetData, (mvvm::SessionItem * item, const mvvm::variant_t &value, int role),
              (override));

  MOCK_METHOD(mvvm::SessionItem *, FindItem, (const std::string &id), (const, override));

  MOCK_METHOD(void, Clear, (), (override));

  MOCK_METHOD(void, ReplaceRootItem, (std::unique_ptr<mvvm::SessionItem>), (override));

  MOCK_METHOD(void, CheckIn, (mvvm::SessionItem *), (override));

  MOCK_METHOD(void, CheckOut, (mvvm::SessionItem *), (override));
};

}  // namespace mvvm::test

#endif  // MVVM_TEST_MOCK_MODEL_H_
