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

#include "mvvm/model/compounditem.h"
#include "mvvm/model/sessionmodel.h"
#include "mvvm/serialization/xmldocument.h"

#include <iostream>

using namespace ModelView;

//! Create a model with single compound item in it, saves the content in xml file.

int main()
{
  SessionModel model;
  auto item = model.insertItem<CompoundItem>();
  item->AddProperty("Greeting", "Hello");
  item->AddProperty("Addressee", "World!");
  item->AddProperty("Number", 42);

  std::cout << item->Property<std::string>("Greeting") << " "
            << item->Property<std::string>("Addressee") << "\n";

  XmlDocument document({&model});
  document.Save("test.xml");

  return 0;
}
