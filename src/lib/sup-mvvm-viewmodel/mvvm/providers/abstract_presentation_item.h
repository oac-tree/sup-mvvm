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

#ifndef MVVM_PROVIDERS_ABSTRACT_PRESENTATION_ITEM_H_
#define MVVM_PROVIDERS_ABSTRACT_PRESENTATION_ITEM_H_

#include <mvvm/providers/i_viewitem_data.h>

namespace mvvm
{

class SessionItem;

/**
 * @brief The AbstractPresentationItem class provides an access to SessionItem data.
 *
 * It partially implements IViewItemData and serves as  a common base for all presentations of a
 * single SessionItem in a cell of Qt's trees and tables.
 */
class AbstractPresentationItem : public IViewItemData
{
public:
  explicit AbstractPresentationItem(SessionItem* item);

  bool IsEnabled() const override;

  bool IsEditable() const override;

  bool IsCheckable() const override;

  SessionItem* GetItem();

  const SessionItem* GetItem() const;

  /**
   * @brief Returns vector of Qt data roles corresponding to given SessionItem's role.
   *
   * The method is used in the context, when item's role is changed, and we want to know which Qt
   * roles should be emitted by the view model to notify all views.
   */
  virtual QVector<role_t> GetQtRolesToEmit(role_t data_role) const = 0;

private:
  SessionItem* m_item{nullptr};  // the item to present
};

}  // namespace mvvm

#endif  // MVVM_PROVIDERS_ABSTRACT_PRESENTATION_ITEM_H_
