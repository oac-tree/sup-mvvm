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
   * @brief Checks if item is enabled.
   *
   * Depending on concrete implementation, the disabled item can be show in gray color.
   * Please note, that it is different from Qt::ItemIsEnabled flag (we have it always true).
   */
  virtual bool IsEnabled() const = 0;

  /**
   * @brief Checks if item is editable.
   *
   * Normally, it is not possible to edit item's value by double-clicking on the cell in the view.
   * The item can still be changed programmatically.
   */
  virtual bool IsEditable() const = 0;

  /**
   * @brief Checks if item is checkable.
   *
   * The behavior is similar to Qt's cell with ItemIsUserCheckable flag set.
   */
  virtual bool IsCheckable() const = 0;

  /**
   * @brief Checks if the given Qt role is a presentation-only role.
   *
   * Presentation-only roles (e.g. background highlighting) are stored on board of the view item
   * data and are never propagated to the underlying SessionItem. As a consequence, setting such a
   * role does not trigger any SessionModel notification, and the view model has to emit dataChanged
   * on its own.
   *
   * @param qt_role Qt role to check.
   */
  virtual bool IsPresentationOnlyRole(role_t qt_role) const = 0;
};

}  // namespace mvvm

#endif  // MVVM_PROVIDERS_I_VIEWITEM_DATA_H_
