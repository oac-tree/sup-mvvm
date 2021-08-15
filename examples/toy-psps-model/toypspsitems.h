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

#ifndef TOYPSPSITEMS_H
#define TOYPSPSITEMS_H

//! @file toypspsitems.h
//! Collection of classes defining main components of PSPS application model.

#include "mvvm/model/compounditem.h"
#include "mvvm/model/sessionmodel.h"

#include <string>

//! Actuator item. Contains reference to one or more PV-variables.

class ActuatorItem : public ModelView::CompoundItem
{
public:
  static inline const std::string Type = "ActuatorItem";
  static inline const std::string P_VARIABLE_NAME = "P_VARIABLE_NAME";
  static inline const std::string P_VARIABLE_VALUE = "P_VARIABLE_VALUE";

  ActuatorItem();

  void SetVariableName(const std::string& pv_name);
  void SetValue(double value);
};

//! The action to be performed on ActuatorItem. Contains the link to particular ActuatorItem
//! and transition function.

class ActionItem : public ModelView::CompoundItem
{
public:
  static inline const std::string Type = "ActionItem";

  static inline const std::string P_ACTUATOR_LINK = "ACTUATOR_LINK";
  static inline const std::string P_TRANSITION_FUNCTION = "T_TRANSITION_FUNCTION";
  static inline const std::string P_DURATION = "P_DURATION";
  static inline const std::string P_START_VALUE = "P_START_VALUE";
  static inline const std::string P_STOP_VALUE = "P_STOP_VALUE";

  ActionItem();

  void LinkToActuator(const ActuatorItem& actuator);
};

//! Pulse schedule segment. Contains collection of ActuatorItems involved into
//! this segment.

class SegmentItem : public ModelView::CompoundItem
{
public:
  static inline const std::string Type = "SegmentItem";
  static inline const std::string T_ACTUATORS = "T_ACTUATORS";

  SegmentItem();
};

//! Transition between segments. Contains collection of ActionItem's to be executed.

class TransitionItem : public ModelView::CompoundItem
{
public:
  static inline const std::string Type = "TransitionItem";
  static inline const std::string T_ACTIONS = "T_ACTIONS";

  TransitionItem();
};

//! Pulse schedule. Contains a collection of SegmentItem's with ActionItem's in-between.

class PulseScheduleItem : public ModelView::CompoundItem
{
public:
  static inline const std::string Type = "PulseScheduleItem";
  static inline const std::string T_ELEMENTS = "T_ELEMENTS";

  PulseScheduleItem();
};

#endif
