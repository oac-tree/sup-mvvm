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

#include "mvvm/model/session_model.h"

#include <mvvm/model/item_constants.h>
#include <mvvm/model/mvvm_types.h>
#include <mvvm/model/property_item.h>

#include <benchmark/benchmark.h>

namespace mvvm::test
{

/**
 * @brief Testing performance of basic operations with SessionModel.
 */
class SessionModelBenchmark : public benchmark::Fixture
{
};

BENCHMARK_F(SessionModelBenchmark, SetData)(benchmark::State &state)
{
  mvvm::SessionModel model;
  auto item = model.InsertItem<PropertyItem>();

  int value{0};
  for (auto dummy : state)
  {
    model.SetData(item, value++, DataRole::kData);
  }
}

BENCHMARK_F(SessionModelBenchmark, SetSameData)(benchmark::State &state)
{
  mvvm::SessionModel model;
  auto item = model.InsertItem<PropertyItem>();

  const int value{0};
  for (auto dummy : state)
  {
    model.SetData(item, value, DataRole::kData);
  }
}

BENCHMARK_F(SessionModelBenchmark, InsertItem)(benchmark::State &state)
{
  mvvm::SessionModel model;
  auto parent = model.GetRootItem();
  const TagIndex tag_index{constants::kRootItemTag, 0};

  for (auto dummy : state)
  {
    auto item = model.InsertItem<PropertyItem>(parent, tag_index);
    state.PauseTiming();
    model.TakeItem(parent, tag_index);
    state.ResumeTiming();
  }
}

BENCHMARK_F(SessionModelBenchmark, TakeItem)(benchmark::State &state)
{
  mvvm::SessionModel model;
  auto parent = model.GetRootItem();
  const TagIndex tag_index{constants::kRootItemTag, 0};

  for (auto dummy : state)
  {
    state.PauseTiming();
    auto item = model.InsertItem<PropertyItem>(parent, tag_index);
    state.ResumeTiming();
    model.TakeItem(parent, tag_index);
  }
}

BENCHMARK_F(SessionModelBenchmark, InsertAndTake)(benchmark::State &state)
{
  mvvm::SessionModel model;
  auto parent = model.GetRootItem();
  const TagIndex tag_index{constants::kRootItemTag, 0};

  for (auto dummy : state)
  {
    auto item = model.InsertItem<PropertyItem>(parent, tag_index);
    model.TakeItem(parent, tag_index);
  }
}

}  // namespace mvvm::test
