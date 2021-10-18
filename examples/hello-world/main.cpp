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
#include "mvvm/serialization/xmldocument.h"
#include "mvvm/viewmodel/applicationmodel.h"
#include "mvvm/viewmodel/allitemsviewmodel.h"

#include <QTreeView>
#include <QApplication>
#include <QLocale>
#include <iostream>

using namespace ModelView;

//! Create a model with single compound item in it, saves the content in xml file.

int main(int argc, char** argv)
{
  QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));
  QApplication app(argc, argv);

  ApplicationModel model;
  auto item = model.InsertItem<CompoundItem>();
  item->AddProperty("Greeting", "Hello");
  item->AddProperty("Addressee", "World!");
  item->AddProperty("Number", 42);

  std::cout << item->Property<std::string>("Greeting") << " "
            << item->Property<std::string>("Addressee") << "\n";

  AllItemsViewModel viewmodel(&model);

  QTreeView view;
  view.setModel(&viewmodel);
  view.show();

  auto result = app.exec();

  XmlDocument document({&model});
  document.Save("test.xml");

  return result;
}
