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

#include "mvvm/model/application_model.h"

#include <benchmark/benchmark.h>
#include <mvvm/model/mvvm_types.h>
#include <mvvm/model/property_item.h>
#include <mvvm/model/item_constants.h>

using namespace mvvm;

//! Testing performance of basic operations with ApplicationModel.

class ApplicationModelBenchmark : public benchmark::Fixture
{
};

BENCHMARK_F(ApplicationModelBenchmark, SetData)(benchmark::State &state)
{
  mvvm::ApplicationModel model;
  auto item = model.InsertItem<PropertyItem>();

  int value{0};
  for (auto dummy : state)
  {
    model.SetData(item, value++, DataRole::kData);
  }
}

BENCHMARK_F(ApplicationModelBenchmark, SetSameData)(benchmark::State &state)
{
  mvvm::ApplicationModel model;
  auto item = model.InsertItem<PropertyItem>();

  int value{0};
  for (auto dummy : state)
  {
    model.SetData(item, value, DataRole::kData);
  }
}

//! Measuring performance of insert item.
BENCHMARK_F(ApplicationModelBenchmark, InsertItem)(benchmark::State &state)
{
  mvvm::ApplicationModel model;
  auto parent = model.GetRootItem();
  TagIndex tag_index{constants::kRootItemTag, 0};

  int value{0};
  for (auto dummy : state)
  {
    auto item = model.InsertItem<PropertyItem>(parent, tag_index);
    state.PauseTiming();
    model.TakeItem(parent, tag_index);
    state.ResumeTiming();
  }
}

//! Measuring performance of insert item.
BENCHMARK_F(ApplicationModelBenchmark, TakeItem)(benchmark::State &state)
{
  mvvm::ApplicationModel model;
  auto parent = model.GetRootItem();
  TagIndex tag_index{constants::kRootItemTag, 0};

  int value{0};
  for (auto dummy : state)
  {
    state.PauseTiming();
    auto item = model.InsertItem<PropertyItem>(parent, tag_index);
    state.ResumeTiming();
    model.TakeItem(parent, tag_index);
  }
}

//! Measuring performance of insert/take item.
BENCHMARK_F(ApplicationModelBenchmark, InsertAndTake)(benchmark::State &state)
{
  mvvm::ApplicationModel model;
  auto parent = model.GetRootItem();
  TagIndex tag_index{constants::kRootItemTag, 0};

  int value{0};
  for (auto dummy : state)
  {
    auto item = model.InsertItem<PropertyItem>(parent, tag_index);
    model.TakeItem(parent, tag_index);
  }
}
