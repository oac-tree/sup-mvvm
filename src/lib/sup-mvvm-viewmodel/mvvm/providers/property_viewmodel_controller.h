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

#ifndef MVVM_PROVIDERS_PROPERTY_VIEWMODEL_CONTROLLER_H_
#define MVVM_PROVIDERS_PROPERTY_VIEWMODEL_CONTROLLER_H_

#include <mvvm/providers/viewmodel_controller.h>

namespace mvvm
{

class ISessionModel;
class ViewModelBase;
class SessionItem;

/**
 * @brief The PropertyViewModelController class is a controller intended for PropertyViewModel.
 *
 * It populates the view model using PropertyItemsStrategy to look for children, and
 * LabelDataRowStrategy to construct rows.
 *
 * It additionally handles GroupItem: the property items shown beneath a group are those of its
 * currently selected item (this part is handled by PropertyItemsStrategy). When the selection
 * changes, the controller regenerates the whole branch of views representing the group.
 */
class PropertyViewModelController : public ViewModelController
{
public:
  PropertyViewModelController(ISessionModel* model, ViewModelBase* view_model);

  void OnDataChangedEvent(const DataChangedEvent& event) override;

private:
  /**
   * @brief Regenerates the branch of views representing the given group item.
   *
   * The branch is removed and recreated from scratch by replaying remove/insert events. Recreation
   * goes through the children strategy, which reports the property items of the newly selected
   * item.
   */
  void RebuildGroupBranch(SessionItem& group);
};

}  // namespace mvvm

#endif  // MVVM_PROVIDERS_PROPERTY_VIEWMODEL_CONTROLLER_H_
