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

#include "viewitem_factory.h"

#include "boolean_data_presentation_item.h"
#include "default_data_presentation_item.h"
#include "default_display_name_presentation_item.h"
#include "standard_presentation_items.h"

#include <mvvm/model/session_item.h>
#include <mvvm/providers/viewitem.h>

namespace mvvm
{

std::unique_ptr<ViewItem> CreateRootViewItem(SessionItem* item)
{
  return CreateLabelViewItem(item);
}

std::unique_ptr<ViewItem> CreateLabelViewItem(SessionItem* item, const std::string& label)
{
  auto presentation = std::make_unique<LabelPresentationItem>(item, label);
  return std::make_unique<ViewItem>(std::move(presentation));
}

std::unique_ptr<ViewItem> CreateDisplayNameViewItem(SessionItem* item)
{
  auto presentation = std::make_unique<DefaultDisplayNamePresentationItem>(
      item, DefaultDisplayNamePresentationItem::EditMode::kAuto);
  return std::make_unique<ViewItem>(std::move(presentation));
}

std::unique_ptr<ViewItem> CreateDataViewItem(SessionItem* item, role_t role)
{
  if (item == nullptr)
  {
    return nullptr;
  }

  auto value = item->Data(role);
  std::unique_ptr<AbstractPresentationItem> presentation;

  // a nullable bool is edited with a combo, not with a checkbox
  if ((GetTypeCode(value) == TypeCode::Bool) && !item->HasFlag(Appearance::kNullable))
  {
    presentation = std::make_unique<BooleanDataPresentationItem>(item);
  }
  else
  {
    presentation = std::make_unique<DefaultDataPresentationItem>(item, role);
  }

  return std::make_unique<ViewItem>(std::move(presentation));
}

std::unique_ptr<ViewItem> CreateEditableDisplayNameViewItem(SessionItem* item)
{
  auto presentation = std::make_unique<DefaultDisplayNamePresentationItem>(
      item, DefaultDisplayNamePresentationItem::EditMode::kEditable);
  return std::make_unique<ViewItem>(std::move(presentation));
}

std::unique_ptr<ViewItem> CreateFixedDataViewItem(SessionItem* item,
                                                  std::map<int32_t, QVariant> fixed_data)
{
  auto presentation = std::make_unique<FixedDataPresentationItem>(item, std::move(fixed_data));
  return std::make_unique<ViewItem>(std::move(presentation));
}

}  // namespace mvvm
