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

#include "mvvm_error_condition.h"

#include <map>

namespace mvvm
{

namespace
{

constexpr auto kUnknownErrorMessage = "unknown error";

/**
 * @brief Returns a map with correspondance of MvvmErrorCondition to the message.
 */
std::map<MvvmErrorCondition, std::string> GetMessageMap()
{
  const std::map<MvvmErrorCondition, std::string> result = {
      {MvvmErrorCondition::ModelError, "model error"}, {MvvmErrorCondition::MaxCount, "MaxCount"}};
  return result;
}

/**
 * @brief The MvvmErrorConditionCategory class provides description for mvvm::MvvmErrorCondition
 * codes.
 *
 * It is also used to create std::error_condition from mvvm::MvvmErrorCondition enums.
 */
class MvvmErrorConditionCategory : public std::error_category
{
public:
  /**
   * @brief The name of the condition category.
   */
  const char* name() const noexcept override { return kModelErrorCondition; }

  /**
   * @brief Returns the error message corresponding to the error value.
   */
  std::string message(int error_value) const override
  {
    const static auto kErrorMessageMap = GetMessageMap();
    auto iter = kErrorMessageMap.find(static_cast<MvvmErrorCondition>(error_value));
    if (iter != kErrorMessageMap.end())
    {
      return iter->second;
    }
    return kUnknownErrorMessage;
  }
};

/**
 * @brief Returns a reference to a global MvvmErrorConditionCategory instance.
 *
 * It's unique address is used to construct error_condition from our error enums.
 */
const MvvmErrorConditionCategory& model_condition_category()
{
  static const MvvmErrorConditionCategory instance;
  return instance;
}

}  // namespace

std::error_condition make_error_condition(MvvmErrorCondition condition)
{
  return {static_cast<std::int32_t>(condition), model_condition_category()};
}

// namespace

}  // namespace mvvm
