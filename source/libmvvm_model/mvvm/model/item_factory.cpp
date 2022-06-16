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

#include "mvvm/model/item_factory.h"

#include <mvvm/model/item_catalogue.h>
#include <mvvm/model/sessionitem.h>

namespace mvvm
{

ItemFactory::ItemFactory(std::unique_ptr<ItemCatalogue<SessionItem>> catalogue)
    : m_catalogue(std::move(catalogue))
{
}

void ItemFactory::RegisterItem(const std::string& item_type, item_factory_func_t func,
                               const std::string& label)
{
  m_catalogue->RegisterItem(item_type, func, label);
}

ItemFactory::~ItemFactory() = default;

std::unique_ptr<SessionItem> ItemFactory::CreateItem(const std::string& item_type) const
{
  return m_catalogue->Create(item_type);
}

}  // namespace mvvm
