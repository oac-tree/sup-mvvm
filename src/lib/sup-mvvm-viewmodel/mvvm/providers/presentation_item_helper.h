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

#ifndef MVVM_PROVIDERS_PRESENTATION_ITEM_HELPER_H_
#define MVVM_PROVIDERS_PRESENTATION_ITEM_HELPER_H_

//! @file
//! Collection of helper function for PresentationItem.

#include <mvvm/core/mvvm_types.h>

#include <QVariant>

namespace mvvm
{

class SessionItem;

/**
 * @brief Returns the numeric value of a Qt item-data role as role_t.
 *
 * Centralizes the explicit conversion required to compare Qt's unscoped
 * Qt::ItemDataRole against our integer role_t (MISRA C++ 2023 Rule 10.2.3
 * forbids implicit use of the numeric value of an unscoped enumeration).
 *
 * @param qt_role A Qt item-data role.
 * @return Numeric value of the role as role_t.
 */
constexpr role_t GetQtRole(Qt::ItemDataRole qt_role)
{
  return static_cast<role_t>(qt_role);
}

/**
 * @brief Returns QString representing display role for given data role.
 *
 * This function is used when Qt asks the data for Qt::DisplayRole.
 *
 * @param item An item which carries data for given data_role.
 * @param data_role A data role which we want to present.
 * @return QString Representation
 */
QVariant GetItemDataDisplayRole(const SessionItem& item, role_t data_role);

}  // namespace mvvm

#endif  // MVVM_PROVIDERS_PRESENTATION_ITEM_HELPER_H_
