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

#ifndef MVVM_MODEL_SESSION_ITEM_DATA_H_
#define MVVM_MODEL_SESSION_ITEM_DATA_H_

#include <mvvm/core/variant.h>

#include <map>

namespace mvvm
{

/**
 * @brief The SessionItemData class represents a container with pairs of <data, role>.
 */
class MVVM_MODEL_EXPORT SessionItemData
{
public:
  using container_t = std::map<std::int32_t, variant_t>;
  using const_iterator = container_t::const_iterator;

  /**
   * @brief Returns vector of all roles for which data exists.
   */
  std::vector<std::int32_t> GetRoles() const;

  /**
   * @brief Returns data for a given role, if exist.
   *
   * Will return non-initialized variant otherwise.
   */
  variant_t Data(std::int32_t role) const;

  /**
   * @brief Sets the data for a given role and returns true if data was changed.
   *
   * Will return false, if the given value was already set. If the new value is invalid (i.e.
   * contains variant in monostate), the old value for this role will be removed.
   *
   * If the new variant is incompatible with the existing variant (i.e. has an underlying type that
   * differs from current type), exception will be thrown. This means that it is not possible to
   * change the type of variant, once the role was set.
   *
   * @see also utils::ReplaceData
   */
  bool SetData(const variant_t& value, std::int32_t role);

  /**
   * @brief Checks if the data for a given role exists.
   */
  bool HasData(std::int32_t role) const;

  const_iterator begin() const;

  const_iterator end() const;

private:
  /**
   * @brief Makes sure that the old variant is compatible with the new variant for given role.
   *
   * Throws an exception if variants are incompatible.
   */
  void AssureCompatibility(const variant_t& old_value, const variant_t& new_value,
                           std::int32_t role) const;

  container_t m_values;
};

}  // namespace mvvm

#endif  // MVVM_MODEL_SESSION_ITEM_DATA_H_
