/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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
const std::size_t kDepth{6};
}  // namespace

/**
 * @brief Testing performance of functions from model_utils.cpp
 */
class ModelUtilsBenchmark : public benchmark::Fixture
{
public:
};

BENCHMARK_F(ModelUtilsBenchmark, PathFromItem)(benchmark::State& state)
{
  auto test_data = test::CreateTestData(kDepth, kMaxChildrenCount);

  for (auto dummy : state)
  {
    for (const auto& info : test_data->item_info)
    {
      const auto path = utils::PathFromItem(info.item);
    }
  }
}

BENCHMARK_F(ModelUtilsBenchmark, ItemFromPath)(benchmark::State& state)
{
  auto test_data = test::CreateTestData(kDepth, kMaxChildrenCount);

  for (auto dummy : state)
  {
    for (const auto& info : test_data->item_info)
    {
      const auto item = utils::ItemFromPath(*test_data->model, info.path);
    }
  }
}

BENCHMARK_F(ModelUtilsBenchmark, FindItem)(benchmark::State& state)
{
  auto test_data = test::CreateTestData(kDepth, kMaxChildrenCount);

  for (auto dummy : state)
  {
    for (const auto& info : test_data->item_info)
    {
      const auto item = test_data->model->FindItem(info.identifier);
    }
  }
}

}  // namespace mvvm::test
