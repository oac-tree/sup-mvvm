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
  MOCK_METHOD(mvvm::SessionItem *, GetRootItem, (), (const, override));

  MOCK_METHOD(mvvm::SessionItem *, InsertItem,
              (std::unique_ptr<mvvm::SessionItem> item, mvvm::SessionItem *parent,
               const mvvm::TagIndex &tag_index),
              (override));

  MOCK_METHOD(mvvm::SessionItem *, InsertNewItem,
              (const std::string &item_type, mvvm::SessionItem *parent,
               const mvvm::TagIndex &tag_index),
              (override));

  MOCK_METHOD(std::unique_ptr<mvvm::SessionItem>, TakeItem,
              (mvvm::SessionItem * parent, const mvvm::TagIndex &tag_index), (override));

  MOCK_METHOD(void, RemoveItem, (mvvm::SessionItem * item), (override));

  MOCK_METHOD(void, MoveItem,
              (mvvm::SessionItem * item, mvvm::SessionItem *new_parent,
               const mvvm::TagIndex &tag_index),
              (override));

  MOCK_METHOD(bool, SetData, (mvvm::SessionItem * item, const variant_t &value, int role),
              (override));

  MOCK_METHOD(void, Clear, (std::function<void(mvvm::SessionItem *)>), (override));
};

class MockModelDecorator : public mvvm::SessionModelInterface
{
public:
  MockModelDecorator(MockModel *mock_model) : m_mock_model(mock_model) {}

  std::string GetType() const { return m_mock_model->GetType(); }

  mvvm::SessionItem *GetRootItem() const { return m_mock_model->GetRootItem(); }

  mvvm::SessionItem *InsertItem(std::unique_ptr<mvvm::SessionItem> item, mvvm::SessionItem *parent,
                                const mvvm::TagIndex &tag_index)
  {
    return m_mock_model->InsertItem(std::move(item), parent, tag_index);
  }

  mvvm::SessionItem *InsertNewItem(const std::string &item_type, mvvm::SessionItem *parent,
                                   const mvvm::TagIndex &tag_index)
  {
    return m_mock_model->InsertNewItem(item_type, parent, tag_index);
  }

  std::unique_ptr<mvvm::SessionItem> TakeItem(mvvm::SessionItem *parent,
                                              const mvvm::TagIndex &tag_index)
  {
    return m_mock_model->TakeItem(parent, tag_index);
  }

  void RemoveItem(mvvm::SessionItem *item) { m_mock_model->RemoveItem(item); }

  void MoveItem(mvvm::SessionItem *item, mvvm::SessionItem *new_parent,
                const mvvm::TagIndex &tag_index)
  {
    m_mock_model->MoveItem(item, new_parent, tag_index);
  }

  bool SetData(mvvm::SessionItem *item, const variant_t &value, int role)
  {
    return m_mock_model->SetData(item, value, role);
  }

  void Clear(std::function<void(mvvm::SessionItem *)> callback)
  {
    return m_mock_model->Clear(callback);
  }

  MockModel *m_mock_model{nullptr};
};

#endif  // TESTS_LIBTESTMACHINERY_MOCK_MODEL_LISTENER_H_
