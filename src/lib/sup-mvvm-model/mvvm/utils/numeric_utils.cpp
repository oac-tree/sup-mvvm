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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "numeric_utils.h"

#include <algorithm>
#include <limits>
#include <random>

namespace mvvm::utils
{

bool AreAlmostEqual(double a, double b, double tolerance)
{
  constexpr double eps = std::numeric_limits<double>::epsilon();
  return std::abs(a - b) <= eps * std::max(tolerance * eps, std::max(1., tolerance) * std::abs(b));
}

std::int32_t RandInt(std::int32_t low, std::int32_t high)
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<std::int32_t> uniform_int(low, high);
  return uniform_int(gen);
}

double RandDouble(double low, double high)
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> uniform_real(low, high);
  return uniform_real(gen);
}

}  // namespace mvvm::utils
