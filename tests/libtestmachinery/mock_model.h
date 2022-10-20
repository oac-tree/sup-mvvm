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
#include <mvvm/model/sessionitem.h>

#include <memory>

//! Mocking class for SessionModelInterface.

class MockModel : public mvvm::SessionModelInterface
{
public:
  // Old google test doesn't support MOCK_METHOD, and unique_ptr in arguments.

  //  MOCK_METHOD(std::string, GetType, (), (const, override));
  MOCK_CONST_METHOD0(GetType, std::string());

  //  MOCK_METHOD(mvvm::SessionItem *, GetRootItem, (), (const, override));
  MOCK_CONST_METHOD0(GetRootItem, mvvm::SessionItem *());

  //  MOCK_METHOD(const mvvm::ItemFactoryInterface *, GetFactory, (), (const, override));
  MOCK_CONST_METHOD0(GetFactory, const mvvm::ItemFactoryInterface *());

  //  MOCK_METHOD(mvvm::ModelEventSubscriberInterface *, GetSubscriber, (), (const, override));
  MOCK_CONST_METHOD0(GetEventHandler, mvvm::ModelEventHandler *());

  //  MOCK_METHOD(mvvm::SessionItem *, InsertItem,
  //              (std::unique_ptr<mvvm::SessionItem> item, mvvm::SessionItem *parent,
  //               const mvvm::TagIndex &tag_index),
  //              (override));

  // Old gtest doesn't support unique_ptr in mock methods
  mvvm::SessionItem* InsertItem(std::unique_ptr<mvvm::SessionItem> item, mvvm::SessionItem* parent,
                                  const mvvm::TagIndex& tag_index)
  {
    return nullptr;
  }


  //  MOCK_METHOD(std::unique_ptr<mvvm::SessionItem>, TakeItem,
  //              (mvvm::SessionItem * parent, const mvvm::TagIndex &tag_index), (override));
  MOCK_METHOD2(TakeItem, std::unique_ptr<mvvm::SessionItem>(mvvm::SessionItem *parent,
                                                            const mvvm::TagIndex &tag_index));

  //  MOCK_METHOD(void, RemoveItem, (mvvm::SessionItem * item), (override));
  MOCK_METHOD1(RemoveItem, void(mvvm::SessionItem *item));

  //  MOCK_METHOD(void, MoveItem,
  //              (mvvm::SessionItem * item, mvvm::SessionItem *new_parent,
  //               const mvvm::TagIndex &tag_index),
  //              (override));
  MOCK_METHOD3(MoveItem, void(mvvm::SessionItem *item, mvvm::SessionItem *new_parent,
                              const mvvm::TagIndex &tag_index));

  //  MOCK_METHOD(bool, SetData, (mvvm::SessionItem * item, const variant_t &value, int role),
  //              (override));
  MOCK_METHOD3(SetData, bool(mvvm::SessionItem *item, const variant_t &value, int role));

  //  MOCK_METHOD(mvvm::SessionItem *, FindItem, (const std::string &id), (const, override));
  MOCK_CONST_METHOD1(FindItem, mvvm::SessionItem *(const std::string &id));

  //  MOCK_METHOD(void, Clear, (std::unique_ptr<mvvm::SessionItem>), (override));

  // Old gtest doesn't support unique_ptr in mock methods
  void Clear(std::unique_ptr<mvvm::SessionItem> root_item) {};

  //  MOCK_METHOD(void, CheckIn, (mvvm::SessionItem *), (override));
  MOCK_METHOD1(CheckIn, void(mvvm::SessionItem *));

  //  MOCK_METHOD(void, CheckOut, (mvvm::SessionItem *), (override));
  MOCK_METHOD1(CheckOut, void(mvvm::SessionItem *));
};

//! A decorator to wrap MockModel for later use with unique_ptr (gmock doesn't like
//! to put mocking objects into unique_ptr).

class MockModelDecorator : public mvvm::SessionModelInterface
{
public:
  explicit MockModelDecorator(MockModel *mock_model) : m_mock_model(mock_model) {}

  std::string GetType() const { return m_mock_model->GetType(); }

  mvvm::SessionItem *GetRootItem() const { return m_mock_model->GetRootItem(); }

  const mvvm::ItemFactoryInterface *GetFactory() const { return m_mock_model->GetFactory(); }

  mvvm::ModelEventHandler *GetEventHandler() const
  {
    return m_mock_model->GetEventHandler();
  }

  mvvm::SessionItem *InsertItem(std::unique_ptr<mvvm::SessionItem> item, mvvm::SessionItem *parent,
                                const mvvm::TagIndex &tag_index)
  {
    return m_mock_model->InsertItem(std::move(item), parent, tag_index);
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

  mvvm::SessionItem *FindItem(const std::string &id) const { return m_mock_model->FindItem(id); }

  void Clear(std::unique_ptr<mvvm::SessionItem> root_item)
  {
    m_mock_model->Clear(std::move(root_item));
  }

  void CheckIn(mvvm::SessionItem *item) { return m_mock_model->CheckIn(item); }

  void CheckOut(mvvm::SessionItem *item) { return m_mock_model->CheckOut(item); }

  MockModel *m_mock_model{nullptr};
};

#endif  // TESTS_LIBTESTMACHINERY_MOCK_MODEL_LISTENER_H_
