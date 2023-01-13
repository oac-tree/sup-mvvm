/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/viewmodel/viewitem_factory.h"

#include <mvvm/viewmodel/standard_presentation_items.h>
#include <mvvm/viewmodelbase/viewitem.h>

namespace mvvm
{

std::unique_ptr<ViewItem> CreateRootViewItem(SessionItem* item)
{
  auto presentation = std::make_unique<LabelPresentationItem>(item);
  return std::make_unique<ViewItem>(std::move(presentation));
}

std::unique_ptr<ViewItem> CreateDisplayNameViewItem(SessionItem* item)
{
  auto presentation = std::make_unique<DisplayNamePresentationItem>(item);
  return std::make_unique<ViewItem>(std::move(presentation));
}

std::unique_ptr<ViewItem> CreateDataViewItem(SessionItem* item)
{
  auto presentation = std::make_unique<DataPresentationItem>(item);
  return std::make_unique<ViewItem>(std::move(presentation));
}

std::unique_ptr<ViewItem> CreateEditableDisplayNameViewItem(SessionItem* item)
{
  auto presentation = std::make_unique<EditableDisplayNamePresentationItem>(item);
  return std::make_unique<ViewItem>(std::move(presentation));
}

}  // namespace mvvm
