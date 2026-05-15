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

#ifndef MVVM_PROVIDERS_DEFAULT_DISPLAY_NAME_PRESENTATION_ITEM_H_
#define MVVM_PROVIDERS_DEFAULT_DISPLAY_NAME_PRESENTATION_ITEM_H_

#include <mvvm/providers/abstract_presentation_item.h>

namespace mvvm
{

class SessionItem;

/**
 * @brief The DefaultDisplayNamePresentationItem class presents display name of SessionItem.
 */
class DefaultDisplayNamePresentationItem : public AbstractItemPresentation
{
public:
  explicit DefaultDisplayNamePresentationItem(SessionItem* item);

  bool IsEditable() const override;

  bool IsCheckable() const override;

  QVariant Data(role_t qt_role) const override;

  bool SetData(const QVariant& data, role_t qt_role) override;

  QVector<role_t> GetQtRolesToEmit(role_t data_role) const override;

  role_t GetMainDataRole() const;

private:
  role_t m_main_data_role{0};
};

}  // namespace mvvm

#endif  // MVVM_PROVIDERS_DEFAULT_DISPLAY_NAME_PRESENTATION_ITEM_H_
