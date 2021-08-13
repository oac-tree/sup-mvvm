/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Authors       : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_MODEL_ITEMFACTORY_H
#define MVVM_MODEL_ITEMFACTORY_H

#include "mvvm/interfaces/itemfactoryinterface.h"

#include <memory>

namespace ModelView
{
class ItemCatalogue;

//! Default SessionItem factory.

class MVVM_MODEL_EXPORT ItemFactory : public ItemFactoryInterface
{
public:
  ItemFactory();
  ItemFactory(std::unique_ptr<ItemCatalogue> catalogue);
  ~ItemFactory() override;

  void RegisterItem(const std::string& model_type, item_factory_func_t func,
                    const std::string& label) override;

  std::unique_ptr<SessionItem> CreateItem(const std::string& model_type) const override;

protected:
  std::unique_ptr<ItemCatalogue> m_catalogue;
};

}  // namespace ModelView

#endif  // MVVM_MODEL_ITEMFACTORY_H
