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

#ifndef MVVM_MODEL_ITEM_FACTORY_H_
#define MVVM_MODEL_ITEM_FACTORY_H_

#include <mvvm/interfaces/item_factory_interface.h>

#include <memory>

namespace mvvm
{

template <typename T>
class ItemCatalogue;

/**
 * @brief The ItemFactory class represent a factory of SessionItems.
 */

class MVVM_MODEL_EXPORT ItemFactory : public ItemFactoryInterface
{
public:
  explicit ItemFactory(std::unique_ptr<ItemCatalogue<SessionItem>> catalogue);

  ~ItemFactory() override;

  using ItemFactoryInterface::RegisterItem;

  void RegisterItem(const std::string& type_name, item_factory_func_t func,
                    const std::string& label) override;

  std::unique_ptr<SessionItem> CreateItem(const std::string& type_name) const override;

  std::vector<std::string> GetItemTypes() const override;

private:
  std::unique_ptr<ItemCatalogue<SessionItem>> m_catalogue;
};

/**
 * @brief Returns a reference to global item factory.
 */
ItemFactory& GetGlobalItemFactory();

}  // namespace mvvm

#endif  // MVVM_MODEL_ITEM_FACTORY_H_
