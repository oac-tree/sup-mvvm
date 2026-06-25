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

#include "property_viewmodel_controller.h"

#include "standard_children_strategies.h"
#include "standard_row_strategies.h"

#include <mvvm/model/session_item.h>
#include <mvvm/standarditems/group_item.h>

#include <memory>

namespace mvvm
{

PropertyViewModelController::PropertyViewModelController(ISessionModel* model,
                                                         ViewModelBase* view_model)
    : ViewModelController(view_model, std::make_unique<PropertyItemsStrategy>(),
                          std::make_unique<LabelDataRowStrategy>())
{
  if (model != nullptr)
  {
    SetModel(model);
  }
}

void PropertyViewModelController::OnDataChangedEvent(const DataChangedEvent& event)
{
  // Default behavior: notify the views about the changed data. For a GroupItem this updates the
  // combo cell representing the current selection.
  ViewModelController::OnDataChangedEvent(event);

  // A change of the current item of a group changes the set of property items shown beneath it,
  // so the whole branch of views has to be regenerated.
  if (auto* group = dynamic_cast<GroupItem*>(event.item); group != nullptr)
  {
    RebuildGroupBranch(*group);
  }
}

void PropertyViewModelController::RebuildGroupBranch(SessionItem& group)
{
  auto* parent = group.GetParent();

  // The group is expected to be a property child of a parent which already has a view. We don't
  // handle the case when the group itself serves as the displayed root item: its parent has no
  // view, and OnAboutToRemoveItemEvent treats the removal of the root as a full view reset.
  if (parent == nullptr || &group == GetRootItem())
  {
    return;
  }

  const auto tag_index = group.GetTagIndex();

  // Remove all the views of the group branch, then recreate them from scratch.
  OnAboutToRemoveItemEvent({parent, tag_index});
  OnItemInsertedEvent({parent, tag_index});
}

}  // namespace mvvm
