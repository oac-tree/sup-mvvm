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

#include <benchmark/benchmark.h>
#include <mvvm/model/sessionitem.h>

using namespace mvvm;

//! Testing performance of basic operations with SessionItem.

class SessionItemBenchmark : public benchmark::Fixture {
};

BENCHMARK_F(SessionItemBenchmark, SetData)(benchmark::State& state)
{
  mvvm::SessionItem item;

  int value{0};
  for (auto _ : state)
  {
    item.SetData(value++);
  }
}

BENCHMARK_F(SessionItemBenchmark, SetSameData)(benchmark::State& state)
{
  mvvm::SessionItem item;

  int value{0};
  for (auto _ : state)
  {
    item.SetData(value);
  }
}
