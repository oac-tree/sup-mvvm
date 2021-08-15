/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "toypspsmodel.h"

#include "toypspsitems.h"

using namespace ModelView;

PulseScheduleModel::PulseScheduleModel() : SessionModel("PulseScheduleModel")
{
  // register items which are allowed to populate this model
  registerItem<ActuatorItem>();
  registerItem<ActionItem>();
  registerItem<SegmentItem>();
  registerItem<TransitionItem>();
  registerItem<PulseScheduleItem>();

  PopulateModel();
}

//! Populate the model with initial content.
void PulseScheduleModel::PopulateModel()
{
  // Create pulse schedule.
  auto schedule_item = insertItem<PulseScheduleItem>();

  // insert two segments with transition between them
  auto segment0 = insertItem<SegmentItem>(schedule_item);
  auto transition0 = insertItem<TransitionItem>(schedule_item);
  auto segment1 = insertItem<SegmentItem>(schedule_item);

  // preparing content of segment0
  auto actuator0 = insertItem<ActuatorItem>(segment0);
  actuator0->SetVariableName("CTRL-SUP:COS-T4-OPREQ");
  actuator0->SetValue(42);

  // preparing content of segment 1
  // ...

  // adding action to the transition
  auto action0 = insertItem<ActionItem>(transition0);
  action0->LinkToActuator(*actuator0);
}
