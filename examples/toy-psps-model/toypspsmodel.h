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

#ifndef TOYPSPSMODEL_H
#define TOYPSPSMODEL_H

//! @file toypspsmodel.h
//! Collection of classes defining main components of PSPS application model.

#include "mvvm/model/sessionmodel.h"

//! The model containing pulse schedule definition.
class PulseScheduleModel : public mvvm::SessionModel
{
public:
  PulseScheduleModel();

private:
  void PopulateModel();
};

#endif
