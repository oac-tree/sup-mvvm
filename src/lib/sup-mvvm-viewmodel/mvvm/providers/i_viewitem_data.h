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

#ifndef MVVM_PROVIDERS_I_VIEWITEM_DATA_H_
#define MVVM_PROVIDERS_I_VIEWITEM_DATA_H_

#include <mvvm/core/mvvm_types.h>

#include <QVariant>

namespace mvvm
{

/**
 * @brief The ViewItemDataInterface class is an interface to provide ViewItem with the access to the
 * data.
 */
class IViewItemData
{
public:
  IViewItemData() = default;
  virtual ~IViewItemData() = default;

  IViewItemData(const IViewItemData&) = delete;
  IViewItemData& operator=(const IViewItemData&) = delete;
  IViewItemData(IViewItemData&&) = delete;
  IViewItemData& operator=(IViewItemData&&) = delete;

  virtual QVariant Data(role_t role) const = 0;

  virtual bool SetData(const QVariant& data, role_t qt_role) = 0;

  virtual bool IsEnabled() const = 0;

  virtual bool IsEditable() const = 0;
};

}  // namespace mvvm

#endif  // MVVM_PROVIDERS_I_VIEWITEM_DATA_H_
