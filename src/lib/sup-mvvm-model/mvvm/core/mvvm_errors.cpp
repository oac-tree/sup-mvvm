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

#include "mvvm_errors.h"

#include "mvvm_error_condition.h"

#include <map>

namespace mvvm
{

namespace
{

/**
 * @brief Returns a map with correspondance of ModelError to the error message.
 */
std::map<ModelError, std::string> GetModelErrorMessageMap()
{
  const std::map<ModelError, std::string> result = {
      {ModelError::Success, "success"},
      {ModelError::NullItem, "null item"},
      {ModelError::ItemAndParentAreTheSame, "item and parent are the same"},
      {ModelError::ItemHasNoParent, "item has no parent"},
      {ModelError::ItemHasParent, "item already has parent"},
      {ModelError::InvalidParentItem, "invalid parent item"},
      {ModelError::ItemIsAncestorOfNewParent, "item is ancestor of new parent"},
      {ModelError::FailedToInsertItem, "failed to insert item"},
      {ModelError::ItemBelongToDifferentModel, "item belong to different model"},
      {ModelError::FailedToTakeItem, "can't take item from parent"},
      {ModelError::FailedToMoveItem, "can't move item to this place"},
      {ModelError::InvalidChildItem, "invalid child item"},
      {ModelError::MaxCount, "MaxCount"}};
  return result;
}

/**
 * @brief Returns a map with correspondance of ModelError to MvvmErrorCondition.
 */
std::map<ModelError, MvvmErrorCondition> GetModelErrorConditionMap()
{
  const std::map<ModelError, MvvmErrorCondition> result = {
      {ModelError::Success, MvvmErrorCondition::ModelError},
      {ModelError::NullItem, MvvmErrorCondition::ModelError},
      {ModelError::ItemAndParentAreTheSame, MvvmErrorCondition::ModelError},
      {ModelError::ItemHasNoParent, MvvmErrorCondition::ModelError},
      {ModelError::ItemHasParent, MvvmErrorCondition::ModelError},
      {ModelError::InvalidParentItem, MvvmErrorCondition::ModelError},
      {ModelError::ItemIsAncestorOfNewParent, MvvmErrorCondition::ModelError},
      {ModelError::FailedToInsertItem, MvvmErrorCondition::ModelError},
      {ModelError::ItemBelongToDifferentModel, MvvmErrorCondition::ModelError},
      {ModelError::FailedToTakeItem, MvvmErrorCondition::ModelError},
      {ModelError::FailedToMoveItem, MvvmErrorCondition::ModelError},
      {ModelError::InvalidChildItem, MvvmErrorCondition::ModelError},
      {ModelError::MaxCount, MvvmErrorCondition::ModelError}};
  return result;
}

/**
 * @brief The ModelErrorCategory class provides description for mvvm::ModelError codes.
 *
 * It is also used to create std::error_code from mvvm::ModelError enums.
 */
class ModelErrorCategory : public std::error_category
{
public:
  /**
   * @brief The name of the error category.
   */
  const char* name() const noexcept override { return kModelErrorCategory; }

  /**
   * @brief Returns the error message corresponding to the error value.
   */
  std::string message(int error_value) const override
  {
    const static auto kErrorMessageMap = GetModelErrorMessageMap();
    auto iter = kErrorMessageMap.find(static_cast<ModelError>(error_value));
    if (iter != kErrorMessageMap.end())
    {
      return iter->second;
    }
    return kUnknownErrorMessage;
  }

  /**
   * @brief Returns the default error condition corresponding to the error value.
   *
   * All our ModelError codes map to a single MvvmErrorCondition::ModelError condition.
   */
  std::error_condition default_error_condition(int ev) const noexcept override
  {
    const static auto kErrorConditionMap = GetModelErrorConditionMap();
    auto iter = kErrorConditionMap.find(static_cast<ModelError>(ev));
    if (iter != kErrorConditionMap.end())
    {
      return make_error_condition(iter->second);
    }
    return {ev, *this};
  }
};

/**
 * @brief Returns a reference to a global ModelErrorCategory instance.
 *
 * It's unique address is used to construct error codes from our error enums.
 */
const ModelErrorCategory& model_error_category()
{
  static const ModelErrorCategory instance;
  return instance;
}

}  // namespace

std::error_code make_error_code(ModelError model_error)
{
  return {static_cast<int>(model_error), model_error_category()};
}

// namespace

}  // namespace mvvm
