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

#ifndef MVVM_PROVIDERS_VARIANT_DISPLAY_ROLE_VISITOR_H_
#define MVVM_PROVIDERS_VARIANT_DISPLAY_ROLE_VISITOR_H_

#include <mvvm/core/variant.h>
#include <optional>

namespace mvvm
{

class SessionItem;

/**
 * @brief Helper structure to visit variant and report string representation of its value.
 *
 * It is oriented for needs of Qt cell and tables, and used to provide value conversion to
 * Qt::DisplayRole.
 *
 * It will present main data role as a string. It can use other data roles, if necessary (e.g.
 * number of decimals to present a double).
 */
struct VariantDisplayRoleVisitor
{
  /**
   * @brief Main c-tor.
   *
   * @param item An item which has a data for given data_role.
   * @param main_data_role The data role to present.
   */
  VariantDisplayRoleVisitor(const SessionItem& item, role_t main_data_role);

  std::optional<std::string> operator()(std::monostate value) const;

  std::optional<std::string> operator()(mvvm::boolean value) const;

  std::optional<std::string> operator()(mvvm::char8 value) const;

  std::optional<std::string> operator()(mvvm::int8 value) const;

  std::optional<std::string> operator()(mvvm::uint8 value) const;

  std::optional<std::string> operator()(mvvm::int16 value) const;

  std::optional<std::string> operator()(mvvm::uint16 value) const;

  std::optional<std::string> operator()(mvvm::int32 value) const;

  std::optional<std::string> operator()(mvvm::uint32 value) const;

  std::optional<std::string> operator()(mvvm::int64 value) const;

  std::optional<std::string> operator()(mvvm::uint64 value) const;

  std::optional<std::string> operator()(mvvm::float32 value) const;

  std::optional<std::string> operator()(mvvm::float64 value) const;

  std::optional<std::string> operator()(std::string value) const;

  std::optional<std::string> operator()(const std::vector<double>& value) const;

  std::optional<std::string> operator()(const mvvm::ComboProperty& value) const;

  std::optional<std::string> operator()(const mvvm::ExternalProperty& value) const;

  std::optional<std::string> ToString() const;

private:
  const SessionItem& m_item;
  role_t m_main_data_role{0};
};

}  // namespace mvvm

#endif  // MVVM_PROVIDERS_VARIANT_DISPLAY_ROLE_VISITOR_H_
