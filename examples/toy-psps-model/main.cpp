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
#include "toypspsmodel.h"

#include "mvvm/serialization/xmldocument.h"

#include <iostream>

//! Create a model with PulseSchedule, saves the content in xml file.

int main()
{
  PulseScheduleModel model;
  std::cout << "Number of pulses defined " << model.topItems<PulseScheduleItem>().size() << "\n";

  ModelView::XmlDocument document({&model});
  document.Save("test.xml");

  return 0;
}
