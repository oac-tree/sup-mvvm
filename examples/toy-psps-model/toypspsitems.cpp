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

#include "toypspsitems.h"

#include "mvvm/model/taginfo.h"

using namespace ModelView;

// ----------------------------------------------------------------------------
// ActuatorItem
// ----------------------------------------------------------------------------

ActuatorItem::ActuatorItem() : CompoundItem(Type)
{
  addProperty(P_VARIABLE_NAME, "Undefined")->SetDisplayName("PV-name");
  addProperty(P_VARIABLE_VALUE, 0.0)->SetDisplayName("Value");
}

void ActuatorItem::SetVariableName(const std::string &pv_name)
{
  setProperty(P_VARIABLE_NAME, pv_name);
}

void ActuatorItem::SetValue(double value)
{
  setProperty(P_VARIABLE_VALUE, value);
}

// ----------------------------------------------------------------------------
// ActionItem
// ----------------------------------------------------------------------------

ActionItem::ActionItem() : CompoundItem(Type)
{
  addProperty(P_ACTUATOR_LINK, "Undefined");
  addProperty(P_TRANSITION_FUNCTION, "Undefined");
  addProperty(P_DURATION, 42)->SetDisplayName("Duration [sec]");
  addProperty(P_START_VALUE, 0.0)->setToolTip("The value to set at the beginning of action");
  addProperty(P_STOP_VALUE, 1.0)->setToolTip("The value to set at the end of action");
}

//! Associate given action with the actuator.
void ActionItem::LinkToActuator(const ActuatorItem &actuator)
{
  // TODO save identifier of given actuator using LinkedItem machinery
}

// ----------------------------------------------------------------------------
// SegmentItem
// ----------------------------------------------------------------------------

SegmentItem::SegmentItem() : CompoundItem(Type)
{
  // registering tag intended for storage of arbitrary ammount of ActuatorItems
  RegisterTag(TagInfo::CreateUniversalTag(T_ACTUATORS, {ActuatorItem::Type}), /*default*/ true);
}

// ----------------------------------------------------------------------------
// TransitionItem
// ----------------------------------------------------------------------------

TransitionItem::TransitionItem() : CompoundItem(Type)
{
  // registering tag intended for storage of arbitrary ammount of ActionItems
  RegisterTag(TagInfo::CreateUniversalTag(T_ACTIONS, {ActionItem::Type}), /*default*/ true);
}

// ----------------------------------------------------------------------------
// PulseScheduleItem
// ----------------------------------------------------------------------------

PulseScheduleItem::PulseScheduleItem() : CompoundItem(Type)
{
  // registering tag intended for storage of arbitrary ammount of SegmentItems and TransisionItems
  RegisterTag(TagInfo::CreateUniversalTag(T_ELEMENTS, {SegmentItem::Type, TransitionItem::Type}),
              /*default*/ true);
}
