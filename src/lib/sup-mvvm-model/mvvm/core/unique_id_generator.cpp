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

#include "unique_id_generator.h"

#include "uuid.h"

#include <memory>

struct GeneratorData
{
  std::random_device rd;
  std::array<int, std::mt19937::state_size> seed_data{};
  std::unique_ptr<std::seed_seq> seq;
  std::unique_ptr<std::mt19937> generator;
  std::unique_ptr<uuids::uuid_random_generator> gen;

  GeneratorData() noexcept
  {
    std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
    seq = std::make_unique<std::seed_seq>(std::begin(seed_data), std::end(seed_data));
    generator = std::make_unique<std::mt19937>(*seq);
    gen = std::make_unique<uuids::uuid_random_generator>(*generator);
  }
};

namespace mvvm
{

namespace
{
// File-scope (not local) static storage duration to comply with MISRA C++ 2023 Rule 6.7.1. The
// GeneratorData constructor is self-contained (seeds its own RNG), so it has no
// static-initialisation-order dependency on other translation units.
GeneratorData generator_data;
}  // namespace

std::string UniqueIdGenerator::Generate()
{
  return uuids::to_string((*generator_data.gen)());
}

}  // namespace mvvm
