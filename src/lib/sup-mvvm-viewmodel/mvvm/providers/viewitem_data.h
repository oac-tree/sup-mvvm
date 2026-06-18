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

#ifndef MVVM_PROVIDERS_VIEWITEM_DATA_H_
#define MVVM_PROVIDERS_VIEWITEM_DATA_H_

#include <mvvm/providers/i_viewitem_data.h>

#include <QMap>

namespace mvvm
{

/**
 * @brief The ViewItemData class carries the data for ViewItem.
 *
 * This is a default implementation for ViewItem.
 */
class ViewItemData : public IViewItemData
{
public:
  ViewItemData();

  QVariant Data(role_t qt_role) const override;

  bool SetData(const QVariant& data, role_t qt_role) override;

  bool IsEnabled() const override;

  void SetEnabled(bool value);

  bool IsEditable() const override;

  void SetEditable(bool value);

  void SetCheckable(bool value);

  bool IsCheckable() const override;

  bool IsPresentationOnlyRole(role_t qt_role) const override;

private:
  bool m_is_enabled;
  bool m_is_editable;
  bool m_is_checkable;
  QMap<std::int32_t, QVariant> m_data;
};

}  // namespace mvvm

#endif  // MVVM_PROVIDERS_VIEWITEM_DATA_H_
