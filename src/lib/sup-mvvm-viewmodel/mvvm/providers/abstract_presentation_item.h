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

#ifndef MVVM_PROVIDERS_ABSTRACT_PRESENTATION_ITEMS_H_
#define MVVM_PROVIDERS_ABSTRACT_PRESENTATION_ITEMS_H_

#include <mvvm/providers/i_viewitem_data.h>

namespace mvvm
{

class SessionItem;

/**
 * @brief The AbstractItemPresentation class partially implements IViewItemData.
 *
 * It is a common base for all presentation representing a single SessionItem in a cell of Qt's
 * trees and tables.
 */
class AbstractItemPresentation : public IViewItemData
{
public:
  explicit AbstractItemPresentation(SessionItem* item);

  bool IsEnabled() const override;

  bool IsEditable() const override;

  SessionItem* GetItem();

  const SessionItem* GetItem() const;

private:
  SessionItem* m_item{nullptr};  // the item to present
};

}  // namespace mvvm

#endif  // MVVM_PROVIDERS_ABSTRACT_PRESENTATION_ITEMS_H_
