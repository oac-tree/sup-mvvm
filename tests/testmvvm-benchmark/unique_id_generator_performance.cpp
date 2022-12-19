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

#include "mvvm/core/unique_id_generator.h"

#include <benchmark/benchmark.h>

using namespace mvvm;

//! Testing performance of GUID generator.

class UniqueIdGeneratorBenchmark : public benchmark::Fixture
{
};

BENCHMARK_F(UniqueIdGeneratorBenchmark, Generate)(benchmark::State &state)
{
  int value{0};
  for (auto dummy : state)
  {
    UniqueIdGenerator::Generate();
  }
}

BENCHMARK_F(UniqueIdGeneratorBenchmark, GenerateNoOptimize)(benchmark::State &state)
{
  int value{0};
  for (auto dummy : state)
  {
    benchmark::DoNotOptimize(UniqueIdGenerator::Generate());
    benchmark::ClobberMemory();
  }
}
