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

#include "test_viewmodel_helper.h"

#include <mvvm/model/i_session_model.h>
#include <mvvm/model/property_item.h>
#include <mvvm/viewmodel/all_items_viewmodel.h>

namespace mvvm::test
{

QModelIndex AddDataToModel(const variant_t &data, const std::string &editor_type,
                           AllItemsViewModel &view_model)
{
  auto model = const_cast<ISessionModel *>(view_model.GetModel());
  auto item = model->InsertItem<PropertyItem>();
  item->SetData(data);
  if (!editor_type.empty())
  {
    item->SetEditorType(editor_type);
  }

  // at this point ViewModel was automatically updated, column = 1 is a cell looking to our data
  auto indexes = view_model.GetIndexOfSessionItem(item);
  return indexes.back();  // it should be two indices, the second is looking to our data
}

}  // namespace mvvm::test
