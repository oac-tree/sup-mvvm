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

#ifndef MVVM_PROVIDERS_I_VIEWITEM_DATA_H_
#define MVVM_PROVIDERS_I_VIEWITEM_DATA_H_

#include <mvvm/core/mvvm_types.h>

#include <QVariant>

namespace mvvm
{

/**
 * @brief The IViewItemData class is an interface to provide ViewItem with the access to the
 * data.
 */
class IViewItemData
{
public:
  IViewItemData() = default;
  virtual ~IViewItemData() = default;

  IViewItemData(const IViewItemData&) = delete;
  IViewItemData& operator=(const IViewItemData&) = delete;
  IViewItemData(IViewItemData&&) noexcept = delete;
  IViewItemData& operator=(IViewItemData&&) noexcept = delete;

  /**
   * @brief Returns data for given Qt role.
   *
   * Implements same behavior as in QAbstractItemModel::data() method.
   *
   * @param qt_role Qt role to get data for.
   */
  virtual QVariant Data(role_t qt_role) const = 0;

  /**
   * @brief Sets data for given Qt role.
   *
   * Implements same behavior as in QAbstractItemModel::data() method.
   *
   * @param data The data to set.
   * @param qt_role Qt role to set data for.
   * @return Returns true if data is successfully set, and false otherwise.
   */
  virtual bool SetData(const QVariant& data, role_t qt_role) = 0;

  /**
   * @brief Returns true if item is enabled, and false otherwise.
   *
   * Depending on concrete implementation, the disabled item can be show in gray color.
   */
  virtual bool IsEnabled() const = 0;

  /**
   * @brief Returns true if item is editable, and false otherwise.
   *
   * Normally, it is not possible to edit item's value by double-clicking on the cell in the view.
   */
  virtual bool IsEditable() const = 0;

  /**
   * @brief Returns true if item is checkable, and false otherwise.
   *
   * The behavior is similar to Qt's cell with ItemIsUserCheckable flag set.
   */
  virtual bool IsCheckable() const = 0;
};

}  // namespace mvvm

#endif  // MVVM_PROVIDERS_I_VIEWITEM_DATA_H_
