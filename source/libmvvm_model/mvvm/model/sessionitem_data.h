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

#ifndef MVVM_MODEL_SESSIONITEM_DATA_H_
#define MVVM_MODEL_SESSIONITEM_DATA_H_

#include <mvvm/core/variant.h>

#include <vector>

namespace mvvm
{
//! Handles pairs of <data, role> for SessionItem.
class MVVM_MODEL_EXPORT SessionItemData
{
public:
  using container_t = std::vector<datarole_t>;
  using const_iterator = container_t::const_iterator;

  std::vector<int> GetRoles() const;

  variant_t Data(int role) const;

  bool SetData(const variant_t& value, int role);

  bool HasData(int role) const;

  const_iterator begin() const;

  const_iterator end() const;

private:
  void AssureCompatibility(const variant_t& variant, int role) const;

  container_t m_values;
};

}  // namespace mvvm

#endif  // MVVM_MODEL_SESSIONITEM_DATA_H_
