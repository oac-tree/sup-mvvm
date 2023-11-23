/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/viewmodel/all_items_viewmodel.h"

#include <mvvm/model/application_model.h>
#include <mvvm/test/toy_items.h>

#include <benchmark/benchmark.h>

using namespace mvvm;

//! Testing performance of basic operations with ApplicationModel.

class AllItemsViewModelBenchmark : public benchmark::Fixture
{
public:
  std::unique_ptr<mvvm::test::toyitems::MultiLayerItem> CreateMultiLayer()
  {
    const int max_layer_count = 10;
    auto multilayer = std::make_unique<mvvm::test::toyitems::MultiLayerItem>();
    for (int i = 0; i < max_layer_count; ++i)
    {
      auto layer = multilayer->InsertItem<mvvm::test::toyitems::LayerItem>(mvvm::TagIndex::Append());
      layer->InsertItem<mvvm::test::toyitems::ParticleItem>(mvvm::TagIndex::Append());
    }
    return multilayer;
  }
};

BENCHMARK_F(AllItemsViewModelBenchmark, ComplexInsert)(benchmark::State &state)
{
  mvvm::ApplicationModel model;

  int value{0};
  for (auto dummy : state)
  {
    state.PauseTiming();
    auto multilayer = CreateMultiLayer();
    state.ResumeTiming();
    model.InsertItem(std::move(multilayer), model.GetRootItem(), mvvm::TagIndex::Append());
  }
}

BENCHMARK_F(AllItemsViewModelBenchmark, ComplexInsertWithViewModel)(benchmark::State &state)
{
  mvvm::ApplicationModel model;
  mvvm::AllItemsViewModel viewmodel(&model);

  int value{0};
  for (auto dummy : state)
  {
    state.PauseTiming();
    auto multilayer = CreateMultiLayer();
    state.ResumeTiming();
    model.InsertItem(std::move(multilayer), model.GetRootItem(), mvvm::TagIndex::Append());
  }
}
