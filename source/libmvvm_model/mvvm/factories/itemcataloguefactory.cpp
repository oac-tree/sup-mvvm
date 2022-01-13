/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
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

#include "mvvm/factories/itemcataloguefactory.h"

#include "mvvm/standarditems/standarditemincludes.h"

namespace mvvm
{
std::unique_ptr<ItemCatalogue> CreateStandardItemCatalogue()
{
  auto result = std::make_unique<ItemCatalogue>();
  result->RegisterItem<CompoundItem>();
  result->RegisterItem<ContainerItem>();
  result->RegisterItem<PropertyItem>();
  result->RegisterItem<SessionItem>();
  result->RegisterItem<VectorItem>();
  return result;
}

void AddStandardItemsToCatalogue(ItemCatalogue &user_catalogue)
{
  user_catalogue.merge(*CreateStandardItemCatalogue());
}

}  // namespace ModelView
