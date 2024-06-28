/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "item_factory.h"

#include <mvvm/model/item_catalogue.h>
#include <mvvm/model/session_item.h>
#include <mvvm/standarditems/standard_item_includes.h>

#include <mutex>

namespace mvvm
{
std::once_flag global_item_factory_initialized_flag;

ItemFactory::ItemFactory(std::unique_ptr<ItemCatalogue<SessionItem>> catalogue)
    : m_catalogue(std::move(catalogue))
{
}

void ItemFactory::RegisterItem(const std::string& type_name, item_factory_func_t func,
                               const std::string& label)
{
  m_catalogue->RegisterItem(type_name, func, label);
}

ItemFactory::~ItemFactory() = default;

std::unique_ptr<SessionItem> ItemFactory::CreateItem(const std::string& type_name) const
{
  return m_catalogue->Create(type_name);
}

std::vector<std::string> ItemFactory::GetItemTypes() const
{
  return m_catalogue->GetItemTypes();
}

bool ItemFactory::IsRegistered(const std::string& type_name) const
{
  return m_catalogue->IsRegistered(type_name);
}

void InitItemFactory(ItemFactory& factory)
{
  // basic items
  factory.RegisterItem<CompoundItem>();
  factory.RegisterItem<PropertyItem>();
  factory.RegisterItem<ContainerItem>();
  factory.RegisterItem<SessionItem>();
  factory.RegisterItem<LinkedItem>();
  factory.RegisterItem<VectorItem>();

  // plotting items
  factory.RegisterItem<Data1DItem>();
  factory.RegisterItem<FixedBinAxisItem>();
  factory.RegisterItem<GraphItem>();
  factory.RegisterItem<GraphViewportItem>();
  factory.RegisterItem<PenItem>();
  factory.RegisterItem<PointwiseAxisItem>();
  factory.RegisterItem<TextItem>();
  factory.RegisterItem<ViewportAxisItem>();

  factory.RegisterItem<LineSeriesDataItem>();
  factory.RegisterItem<PointItem>();
  factory.RegisterItem<LineSeriesItem>();
  factory.RegisterItem<ChartViewportItem>();
}

ItemFactory& GetGlobalItemFactory()
{
  static ItemFactory global_item_factory(std::make_unique<ItemCatalogue<SessionItem>>());
  std::call_once(global_item_factory_initialized_flag, InitItemFactory,
                 std::ref(global_item_factory));
  return global_item_factory;
}

}  // namespace mvvm
