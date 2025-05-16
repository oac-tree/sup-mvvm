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

#include <mvvm/model/session_model.h>

#include <benchmark/benchmark.h>

using namespace mvvm;

/**
 * @brief Testing performance of functions from model_utils.cpp
 */
class ModelUtilsBenchmark : public benchmark::Fixture
{
public:
};

BENCHMARK_F(ModelUtilsBenchmark, PathFromItem)(benchmark::State& state) {}
