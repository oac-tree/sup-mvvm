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

#include "mvvm/core/unique_id_generator.h"

#include "uuid.h"

#include <memory>

struct GeneratorData
{
  std::random_device rd;
  std::array<int, std::mt19937::state_size> seed_data{};
  std::unique_ptr<std::seed_seq> seq;
  std::unique_ptr<std::mt19937> generator;
  std::unique_ptr<uuids::uuid_random_generator> gen;

  GeneratorData()
  {
    std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
    seq = std::make_unique<std::seed_seq>(std::begin(seed_data), std::end(seed_data));
    generator = std::make_unique<std::mt19937>(*seq.get());
    gen = std::make_unique<uuids::uuid_random_generator>(*generator.get());
  }
};

namespace mvvm
{

std::string UniqueIdGenerator::Generate()
{
  static GeneratorData data;
  return uuids::to_string((*data.gen)());
}

}  // namespace mvvm
