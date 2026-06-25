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

#include "i_children_strategy.h"
#include "standard_children_strategies.h"
#include "standard_row_strategies.h"
#include "viewitem.h"

#include <mvvm/model/session_item.h>
#include <mvvm/standarditems/group_item.h>
#include <mvvm/viewmodel/viewmodel_base.h>

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
  // so the branch of views beneath the group has to be regenerated.
  if (auto* group = dynamic_cast<GroupItem*>(event.item); group != nullptr)
  {
    RebuildGroupBranch(*group);
  }
}

void PropertyViewModelController::RebuildGroupBranch(SessionItem& group)
{
  auto* group_view = GetViewItemMap().FindView(&group);
  if (group_view == nullptr)
  {
    return;
  }

  // Drop the existing branch beneath the group from the item-view map, keeping the group's own
  // view in place. RemoveRecursively also removes the group itself, so we re-register it.
  (void)GetViewItemMap().RemoveRecursively(&group);
  GetViewItemMap().Insert(&group, group_view);

  // Remove the child rows of the group's view (with notifications), then recreate them. The
  // children strategy now reports the property items of the newly selected item.
  m_view_model->clearRows(group_view);
  for (auto* child : m_children_strategy->GetChildren(&group))
  {
    m_view_model->appendRow(group_view, CreateTreeOfRows(*child));
  }
}

}  // namespace mvvm
