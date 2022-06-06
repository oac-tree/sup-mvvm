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

#include "mvvm/factories/item_catalogue_factory.h"

#include "mvvm/standarditems/standard_item_includes.h"

namespace mvvm
{
std::unique_ptr<ItemCatalogue<SessionItem> > CreateStandardItemCatalogue()
{
  auto result = std::make_unique<ItemCatalogue<SessionItem>>();
  result->RegisterItem<CompoundItem>();
  result->RegisterItem<ContainerItem>();
  result->RegisterItem<Data1DItem>();
  result->RegisterItem<FixedBinAxisItem>();
  result->RegisterItem<GraphItem>();
  result->RegisterItem<GraphViewportItem>();
  result->RegisterItem<LinkedItem>();
  result->RegisterItem<PenItem>();
  result->RegisterItem<PointwiseAxisItem>();
  result->RegisterItem<PropertyItem>();
  result->RegisterItem<SessionItem>();
  result->RegisterItem<TextItem>();
  result->RegisterItem<VectorItem>();
  result->RegisterItem<ViewportAxisItem>();

  return result;
}

void AddStandardItemsToCatalogue(ItemCatalogue<SessionItem> &user_catalogue)
{
  user_catalogue.Merge(*CreateStandardItemCatalogue());
}

}  // namespace mvvm
