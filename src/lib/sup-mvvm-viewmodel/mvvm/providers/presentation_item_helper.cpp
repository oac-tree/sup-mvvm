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

#include "presentation_item_helper.h"

#include "variant_display_role_visitor.h"

namespace mvvm
{

QVariant GetItemDataDisplayRole(const SessionItem& item, role_t data_role)
{
  const VariantDisplayRoleVisitor visitor(item, data_role);
  const auto optional_str = visitor.ToString();
  return optional_str.has_value() ? QVariant(QString::fromStdString(optional_str.value()))
                                  : QVariant();
}

}  // namespace mvvm