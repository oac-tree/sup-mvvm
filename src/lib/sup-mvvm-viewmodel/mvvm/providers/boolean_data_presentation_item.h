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

#ifndef MVVM_PROVIDERS_BOOLEAN_DATA_PRESENTATION_ITEM_H_
#define MVVM_PROVIDERS_BOOLEAN_DATA_PRESENTATION_ITEM_H_

#include <mvvm/providers/default_data_presentation_item.h>

namespace mvvm
{

class SessionItem;

/**
 * @brief The BooleanDataPresentationItem class presents a SessionItem which carries a boolean as
 * main data.
 *
 * ViewItem will be equipped with check box, the modification of main data role will be performed
 * via checkbox.
 */
class BooleanDataPresentationItem : public DefaultDataPresentationItem
{
public:
  explicit BooleanDataPresentationItem(SessionItem* item);

  QVector<role_t> GetQtRolesToEmit(role_t data_role) const override;

  bool IsEditable() const override;

  bool IsCheckable() const override;

  QVariant Data(role_t qt_role) const override;

  bool SetData(const QVariant& data, role_t qt_role) override;
};

}  // namespace mvvm

#endif  // MVVM_PROVIDERS_BOOLEAN_DATA_PRESENTATION_ITEM_H_
