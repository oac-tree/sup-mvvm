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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "viewitem_factory.h"

#include "standard_presentation_items.h"

#include <mvvm/model/session_item.h>
#include <mvvm/viewmodelbase/viewitem.h>

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
  std::unique_ptr<SessionItemPresentation> presentation;
  if (item->HasFlag(Appearance::kEditableDisplayName))
  {
    presentation = std::make_unique<EditableDisplayNamePresentationItem>(item);
  }
  else
  {
    presentation = std::make_unique<DisplayNamePresentationItem>(item);
  }
  return std::make_unique<ViewItem>(std::move(presentation));
}

std::unique_ptr<ViewItem> CreateDataViewItem(SessionItem* item, int role)
{
  auto presentation = std::make_unique<DataPresentationItem>(item, role);
  return std::make_unique<ViewItem>(std::move(presentation));
}

std::unique_ptr<ViewItem> CreateEditableDisplayNameViewItem(SessionItem* item)
{
  auto presentation = std::make_unique<EditableDisplayNamePresentationItem>(item);
  return std::make_unique<ViewItem>(std::move(presentation));
}

std::unique_ptr<ViewItem> CreateFixedDataViewItem(SessionItem* item,
                                                  std::map<int, QVariant> fixed_data)
{
  auto presentation = std::make_unique<FixedDataPresentationItem>(item, std::move(fixed_data));
  return std::make_unique<ViewItem>(std::move(presentation));
}

}  // namespace mvvm
