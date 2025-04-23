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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/model/session_item.h"

#include <mvvm/model/property_item.h>
#include <mvvm/model/taginfo.h>

#include <benchmark/benchmark.h>

using namespace mvvm;

//! Testing performance of basic operations with SessionItem.

class SessionItemBenchmark : public benchmark::Fixture
{
public:
  std::unique_ptr<SessionItem> CreateItem() { return std::make_unique<PropertyItem>(); }
};

//! Measuring performance of item creation.
BENCHMARK_F(SessionItemBenchmark, CreateAndDestroyItem)(benchmark::State &state)
{
  int value{0};
  for (auto dummy : state)
  {
    auto item = std::make_unique<PropertyItem>;
  }
}

//! Measuring performance of item creation wjen optimisation is switched off.
BENCHMARK_F(SessionItemBenchmark, CreateAndDestroyItemNoOptimisation)(benchmark::State &state)
{
  int value{0};
  for (auto dummy : state)
  {
    benchmark::DoNotOptimize(CreateItem());
    benchmark::ClobberMemory();
  }
}

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

//! Measuring performance of insert item.
BENCHMARK_F(SessionItemBenchmark, InsertItem)(benchmark::State &state)
{
  mvvm::SessionItem parent;
  parent.RegisterTag(TagInfo::CreateUniversalTag("tag"),
                     /*set_as_default*/ true);
  TagIndex tag_index{"tag", 0};

  int value{0};
  for (auto dummy : state)
  {
    parent.InsertItem<PropertyItem>(tag_index);
    state.PauseTiming();
    parent.TakeItem(tag_index);
    state.ResumeTiming();
  }
}

//! Measuring performance of insert item.
BENCHMARK_F(SessionItemBenchmark, TakeItem)(benchmark::State &state)
{
  mvvm::SessionItem parent;
  parent.RegisterTag(TagInfo::CreateUniversalTag("tag"),
                     /*set_as_default*/ true);
  TagIndex tag_index{"tag", 0};

  int value{0};
  for (auto dummy : state)
  {
    state.PauseTiming();
    parent.InsertItem<PropertyItem>(tag_index);
    state.ResumeTiming();
    parent.TakeItem(tag_index);
  }
}

//! Measuring performance of insert/take item.
BENCHMARK_F(SessionItemBenchmark, InsertAndTake)(benchmark::State &state)
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
