/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_MODEL_SESSION_ITEM_DATA_H_
#define MVVM_MODEL_SESSION_ITEM_DATA_H_

#include <mvvm/core/variant.h>

#include <vector>

namespace mvvm
{

/**
 * @brief The SessionItemData class represents a container with pairs of <data, role>.
 */
class MVVM_MODEL_EXPORT SessionItemData
{
public:
  using container_t = std::vector<datarole_t>;
  using const_iterator = container_t::const_iterator;

  /**
   * @brief Returns vector of all roles for which data exists.
   */
  std::vector<int> GetRoles() const;

  /**
   * @brief Returns data for a given role, if exist.
   *
   * Will return non-initialized variant otherwise.
   */
  variant_t Data(int role) const;

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
  bool SetData(const variant_t& value, int role);

  /**
   * @brief Checks if the data for a given role exists.
   */
  bool HasData(int role) const;

  const_iterator begin() const;

  const_iterator end() const;

private:
  /**
   * @brief Makes sure that the new variant is compatible with the old variant stored with the given
   * role. Throws exception otherwise.
   */
  void AssureCompatibility(const variant_t& variant, int role) const;

  container_t m_values;
};

}  // namespace mvvm

#endif  // MVVM_MODEL_SESSION_ITEM_DATA_H_
