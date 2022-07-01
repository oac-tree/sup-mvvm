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

#ifndef MVVM_INTERFACES_ITEM_FACTORY_INTERFACE_H_
#define MVVM_INTERFACES_ITEM_FACTORY_INTERFACE_H_

#include <mvvm/model/function_types.h>
#include <mvvm/model_export.h>

namespace mvvm
{
class SessionItem;

//! Interface class for all factories capable of producing SessionItem's.

class MVVM_MODEL_EXPORT ItemFactoryInterface
{
public:
  virtual ~ItemFactoryInterface() = default;

  virtual void RegisterItem(const std::string& item_type, item_factory_func_t func,
                            const std::string& label) = 0;

  virtual std::unique_ptr<SessionItem> CreateItem(const std::string& item_type) const = 0;
};

}  // namespace mvvm

#endif  // MVVM_INTERFACES_ITEM_FACTORY_INTERFACE_H_
