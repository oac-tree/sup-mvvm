/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
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

/**
 * @brief The ItemFactoryInterface class is an interface for all SessionItem factories.
 */

class MVVM_MODEL_EXPORT ItemFactoryInterface
{
public:
  virtual ~ItemFactoryInterface() = default;

  /**
   * @brief Registers item factory function.
   *
   * @param type_name Registration key representing item's type.
   * @param func The factory function.
   * @param label Optional label which should be used in widgets instead of item_type.
   */
  virtual void RegisterItem(const std::string& type_name, item_factory_func_t func,
                            const std::string& label) = 0;

  /**
   * @brief Registers a type.
   *
   * @param label Optional label  which should be used in widgets instead of SessionItyem::Type.
   */
  template <typename T>
  void RegisterItem(const std::string& label = {});

  /**
   * @brief Creates item of given type.
   */
  virtual std::unique_ptr<SessionItem> CreateItem(const std::string& type_name) const = 0;

  /**
   * @brief Returns vector of all registered types.
   */
  virtual std::vector<std::string> GetItemTypes() const = 0;
};

template <typename T>
void ItemFactoryInterface::RegisterItem(const std::string& label)
{
  RegisterItem(T::Type, ItemFactoryFunction<T>, label);
}

}  // namespace mvvm

#endif  // MVVM_INTERFACES_ITEM_FACTORY_INTERFACE_H_
