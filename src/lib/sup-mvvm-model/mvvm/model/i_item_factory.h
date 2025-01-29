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

#ifndef MVVM_MODEL_I_ITEM_FACTORY_H_
#define MVVM_MODEL_I_ITEM_FACTORY_H_

#include <mvvm/model/function_types.h>
#include <mvvm/model_export.h>

namespace mvvm
{

class SessionItem;

/**
 * @brief The IItemFactory class is an interface for all SessionItem factories.
 */
class MVVM_MODEL_EXPORT IItemFactory
{
public:
  virtual ~IItemFactory() = default;

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
   * @param label Optional label  which should be used in widgets instead of SessionItem::Type.
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

  /**
   * @brief Returns true if given type name was registered.
   */
  virtual bool IsRegistered(const std::string& type_name) const = 0;

  /**
   * @brief Returns true if given type was registered.
   */
  template <typename T>
  bool IsRegistered() const;
};

template <typename T>
void IItemFactory::RegisterItem(const std::string& label)
{  
  RegisterItem(T::GetStaticType(), ItemFactoryFunction<T>, label);
}

template <typename T>
bool IItemFactory::IsRegistered() const
{
  return IsRegistered(T::GetStaticType());
}

}  // namespace mvvm

#endif  // MVVM_MODEL_I_ITEM_FACTORY_H_
