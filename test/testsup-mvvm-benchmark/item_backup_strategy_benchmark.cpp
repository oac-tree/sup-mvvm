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

#include "mvvm/factories/item_backup_strategy_factory.h"

#include <mvvm/model/item_factory.h>
#include <mvvm/model/model_utils.h>
#include <mvvm/model/session_model.h>
#include <mvvm/test/model_test_helper.h>

#include <benchmark/benchmark.h>

namespace mvvm::test
{

namespace
{
// 364 items in total, i.e. (3^6-1)/(3-1)
const std::size_t kMaxChildrenCount{3};
const std::size_t kDepth{3};
}  // namespace

/**
 * @brief Testing performance of basic operations with ApplicationModel.
 */
class ItemBackupStrategyBenchmark : public benchmark::Fixture
{
public:
  ItemBackupStrategyBenchmark() { Unit(benchmark::kMillisecond); }
};

BENCHMARK_F(ItemBackupStrategyBenchmark, TreeDataBackupStrategySave)(benchmark::State& state)
{
  auto test_data = test::CreateTestData(kDepth, kMaxChildrenCount);

  auto item_to_store = test_data->model->GetRootItem();

  auto strategy = CreateItemTreeDataBackupStrategy(&GetGlobalItemFactory());

  for (auto dummy : state)
  {
    for (const auto& info : test_data->item_info)
    {
      strategy->SaveItem(*item_to_store);
    }
  }
}

BENCHMARK_F(ItemBackupStrategyBenchmark, TreeDataBackupStrategyRestore)(benchmark::State& state)
{
  auto test_data = test::CreateTestData(kDepth, kMaxChildrenCount);

  auto item_to_store = test_data->model->GetRootItem();

  auto strategy = CreateItemTreeDataBackupStrategy(&GetGlobalItemFactory());
  strategy->SaveItem(*item_to_store);

  for (auto dummy : state)
  {
    for (const auto& info : test_data->item_info)
    {
      auto restored_item = strategy->RestoreItem();
    }
  }
}

BENCHMARK_F(ItemBackupStrategyBenchmark, ItemCloneBackupStrategySave)(benchmark::State& state)
{
  auto test_data = test::CreateTestData(kDepth, kMaxChildrenCount);

  auto item_to_store = test_data->model->GetRootItem();

  auto strategy = CreateItemCloneBackupStrategy();

  for (auto dummy : state)
  {
    for (const auto& info : test_data->item_info)
    {
      strategy->SaveItem(*item_to_store);
    }
  }
}

BENCHMARK_F(ItemBackupStrategyBenchmark, ItemCloneBackupStrategyRestore)(benchmark::State& state)
{
  auto test_data = test::CreateTestData(kDepth, kMaxChildrenCount);

  auto item_to_store = test_data->model->GetRootItem();

  auto strategy = CreateItemCloneBackupStrategy();
  strategy->SaveItem(*item_to_store);

  for (auto dummy : state)
  {
    for (const auto& info : test_data->item_info)
    {
      auto restored_item = strategy->RestoreItem();
    }
  }
}

}  // namespace mvvm::test
