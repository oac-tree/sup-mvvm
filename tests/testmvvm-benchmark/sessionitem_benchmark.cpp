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

#include "mvvm/model/sessionitem.h"

#include <benchmark/benchmark.h>
#include <mvvm/model/property_item.h>
#include <mvvm/model/taginfo.h>

using namespace mvvm;

//! Testing performance of basic operations with SessionItem.

class SessionItemBenchmark : public benchmark::Fixture
{
};

//! Measuring performance of SetData method when data is always different.
BENCHMARK_F(SessionItemBenchmark, SetData)(benchmark::State &state)
{
  mvvm::SessionItem item;

  int value{0};
  for (auto dummy : state)
  {
    item.SetData(value++);
  }
}

//! Measuring performance of SetData method when data is always the same.
BENCHMARK_F(SessionItemBenchmark, SetSameData)(benchmark::State &state)
{
  mvvm::SessionItem item;

  int value{0};
  for (auto dummy : state)
  {
    item.SetData(value);
  }
}

//! Measuring performance of insert/remove item.
BENCHMARK_F(SessionItemBenchmark, InsertRemove)(benchmark::State &state)
{
  mvvm::SessionItem parent;
  parent.RegisterTag(TagInfo::CreateUniversalTag("tag"),
                     /*set_as_default*/ true);
  TagIndex tag_index{"tag", 0};

  int value{0};
  for (auto dummy : state)
  {
    parent.InsertItem<PropertyItem>(tag_index);
    parent.TakeItem(tag_index);
  }
}
