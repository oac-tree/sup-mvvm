/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef LIBTEST_UTILS_TESTUTILS_MOCK_MODEL_H_
#define LIBTEST_UTILS_TESTUTILS_MOCK_MODEL_H_

#include <mvvm/interfaces/sessionmodel_interface.h>
#include <mvvm/model/sessionitem.h>

#include <gmock/gmock.h>

namespace mvvm::test
{

//! Mocking class for SessionModelInterface.

class MockModel : public mvvm::SessionModelInterface
{
public:
  MOCK_METHOD(std::string, GetType, (), (const, override));

  MOCK_METHOD(mvvm::SessionItem *, GetRootItem, (), (const, override));

  MOCK_METHOD(mvvm::ModelEventHandler *, GetEventHandler, (), (const, override));

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

  MOCK_METHOD(void, Clear, (std::unique_ptr<mvvm::SessionItem>), (override));

  MOCK_METHOD(void, CheckIn, (mvvm::SessionItem *), (override));

  MOCK_METHOD(void, CheckOut, (mvvm::SessionItem *), (override));
};

}  // namespace mvvm::test

#endif  // LIBTEST_UTILS_TESTUTILS_MOCK_MODEL_H_
