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

#include "mvvm/viewmodel/abstract_viewmodel_controller.h"

#include <benchmark/benchmark.h>
#include <mvvm/model/application_model.h>
#include <mvvm/model/mvvm_types.h>
#include <mvvm/model/property_item.h>

using namespace mvvm;

//! Testing performance of basic operations when AbstractModelController is attached to
//! ApplicationModel.

class AbstractViewmodelControllerBenchmark : public benchmark::Fixture
{
public:
  class TestController : public AbstractViewModelController
  {
  public:
    const SessionItem* GetRootItem() const override {return nullptr;}
    void SetRootItemImpl(SessionItem* root_item) override {(void)root_item;};
  };
};

BENCHMARK_F(AbstractViewmodelControllerBenchmark, SetData)(benchmark::State &state)
{
  mvvm::ApplicationModel model;
  auto item = model.InsertItem<PropertyItem>();

  TestController controller;
  controller.Subscribe(&model);

  int value{0};
  for (auto dummy : state)
  {
    model.SetData(item, value++, DataRole::kData);
  }
}

BENCHMARK_F(AbstractViewmodelControllerBenchmark, SetSameData)(benchmark::State &state)
{
  mvvm::ApplicationModel model;
  auto item = model.InsertItem<PropertyItem>();

  TestController controller;
  controller.Subscribe(&model);

  int value{0};
  for (auto dummy : state)
  {
    model.SetData(item, value, DataRole::kData);
  }
}

//! Measuring performance of insert item.
BENCHMARK_F(AbstractViewmodelControllerBenchmark, InsertItem)(benchmark::State &state)
{
  mvvm::ApplicationModel model;
  auto parent = model.GetRootItem();
  TagIndex tag_index{"rootTag", 0};

  TestController controller;
  controller.Subscribe(&model);

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
BENCHMARK_F(AbstractViewmodelControllerBenchmark, TakeItem)(benchmark::State &state)
{
  mvvm::ApplicationModel model;
  auto parent = model.GetRootItem();
  TagIndex tag_index{"rootTag", 0};

  TestController controller;
  controller.Subscribe(&model);

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
BENCHMARK_F(AbstractViewmodelControllerBenchmark, InsertAndTake)(benchmark::State &state)
{
  mvvm::ApplicationModel model;
  auto parent = model.GetRootItem();
  TagIndex tag_index{"rootTag", 0};

  TestController controller;
  controller.Subscribe(&model);

  int value{0};
  for (auto dummy : state)
  {
    auto item = model.InsertItem<PropertyItem>(parent, tag_index);
    model.TakeItem(parent, tag_index);
  }
}
