/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include <mvvm/model/application_model.h>
#include <mvvm/model/compound_item.h>
#include <mvvm/serialization/xml_document.h>
#include <mvvm/signals/item_connect.h>
#include <mvvm/viewmodel/all_items_viewmodel.h>

#include <QApplication>
#include <QLocale>
#include <QTreeView>
#include <iostream>

//! Create a model with single compound item in it, saves the content in xml file.

int main(int argc, char** argv)
{
  QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));
  QApplication app(argc, argv);

  mvvm::ApplicationModel model;
  auto item = model.InsertItem<mvvm::CompoundItem>();
  item->AddProperty("Greeting", "Hello");
  item->AddProperty("Addressee", "World!");

  std::cout << item->Property<std::string>("Greeting") << " "
            << item->Property<std::string>("Addressee") << "\n";

  // add two more properties, make "Number" enabled/disabled when "Flag" changes
  auto& number_property = item->AddProperty("Number", 42);
  auto& flag_property = item->AddProperty("Flag", true);
  auto on_property = [&number_property, &flag_property](const mvvm::DataChangedEvent& event)
  { number_property.SetEnabled(flag_property.Data<bool>()); };
  mvvm::connect::Connect<mvvm::DataChangedEvent>(&flag_property, on_property);

  mvvm::AllItemsViewModel viewmodel(&model);

  QTreeView view;
  view.setModel(&viewmodel);
  view.show();

  auto result = app.exec();

  mvvm::XmlDocument document({&model});
  document.Save("test.xml");

  return result;
}
