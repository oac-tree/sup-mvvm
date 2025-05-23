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

#ifndef MVVM_VIEWMODELBASE_VIEWITEM_DATA_INTERFACE_H_
#define MVVM_VIEWMODELBASE_VIEWITEM_DATA_INTERFACE_H_

#include <mvvm/viewmodel_export.h>

#include <QVariant>

namespace mvvm
{

/**
 * @brief The ViewItemDataInterface class is an interface to provide ViewItem with the access to the
 * data.
 */
class MVVM_VIEWMODEL_EXPORT ViewItemDataInterface
{
public:
  virtual ~ViewItemDataInterface() = default;

  virtual QVariant Data(int role) const = 0;

  virtual bool SetData(const QVariant& data, int role) = 0;

  virtual bool IsEnabled() const = 0;

  virtual bool IsEditable() const = 0;
};

}  // namespace mvvm

#endif  // MVVM_VIEWMODELBASE_VIEWITEM_DATA_INTERFACE_H_
