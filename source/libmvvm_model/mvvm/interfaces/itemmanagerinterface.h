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

#ifndef MVVM_INTERFACES_ITEMMANAGERINTERFACE_H
#define MVVM_INTERFACES_ITEMMANAGERINTERFACE_H

#include "mvvm/model_export.h"

#include <memory>
#include <string>

namespace mvvm
{
class SessionItem;
class ItemFactoryInterface;
class ItemPool;

//! Interface class for

class MVVM_MODEL_EXPORT ItemManagerInterface
{
public:
  virtual ~ItemManagerInterface() = default;

  virtual std::unique_ptr<SessionItem> CreateItem(const std::string& model_type) const = 0;

  virtual std::unique_ptr<SessionItem> CreateEmptyItem() const = 0;

  virtual SessionItem* FindItem(const std::string& id) const = 0;

  virtual std::string FindIdentifier(const SessionItem* item) const = 0;

  virtual void RegisterInPool(SessionItem* item) = 0;

  virtual void UnregisterFromPool(SessionItem* item) = 0;

  virtual const ItemPool* GetItemPool() const = 0;

  virtual ItemPool* GetItemPool() = 0;

  virtual const ItemFactoryInterface* GetFactory() const = 0;

  virtual ItemFactoryInterface* GetFactory() = 0;
};

}  // namespace mvvm

#endif  // MVVM_INTERFACES_ITEMMANAGERINTERFACE_H
