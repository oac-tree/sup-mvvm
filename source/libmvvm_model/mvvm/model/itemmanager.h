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

#ifndef MVVM_MODEL_ITEMMANAGER_H
#define MVVM_MODEL_ITEMMANAGER_H

#include "mvvm/interfaces/itemmanagerinterface.h"

namespace mvvm
{

//! Manages item creation/registration for SessionModel.

class MVVM_MODEL_EXPORT ItemManager : public ItemManagerInterface
{
public:
  ItemManager();
  ~ItemManager() override;

  void SetItemFactory(std::unique_ptr<ItemFactoryInterface> factory);
  void SetItemPool(std::shared_ptr<ItemPool> pool);

  std::unique_ptr<SessionItem> CreateItem(const std::string& model_type) const override;

  std::unique_ptr<SessionItem> CreateEmptyItem() const override;

  SessionItem* FindItem(const std::string& id) const override;

  std::string FindIdentifier(const SessionItem* item) const override;

  const ItemPool* GetItemPool() const override;
  ItemPool* GetItemPool() override;

  void RegisterInPool(SessionItem* item) override;
  void UnregisterFromPool(SessionItem* item) override;

  const ItemFactoryInterface* GetFactory() const override;

  ItemFactoryInterface* GetFactory() override;

private:
  std::shared_ptr<ItemPool> m_item_pool;
  std::unique_ptr<ItemFactoryInterface> m_item_factory;
};

}  // namespace mvvm

#endif  // MVVM_MODEL_ITEMMANAGER_H
