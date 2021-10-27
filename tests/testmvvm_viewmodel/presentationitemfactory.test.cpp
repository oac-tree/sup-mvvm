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

#include "mvvm/viewmodel/presentationitemfactory.h"

#include "mockmodelnotifier.h"

#include "mvvm/model/modelcomposer.h"
#include "mvvm/model/sessionitem.h"
#include "mvvm/model/sessionmodel.h"

#include <gtest/gtest.h>

using namespace ModelView;

//! Tests of factory methods for creation of various PresentationItems.

class PresentationItemFactoryTest : public ::testing::Test
{
public:
};

//! Presentation for SessionItem display name in read only mode.

TEST_F(PresentationItemFactoryTest, DisplayNamePresentation)
{
  SessionItem item;
  item.SetDisplayName("abc");

  auto presentation = CreateDisplayNamePresentation(&item);

  // presentation has a display role, which coincide with SessionItem::GetDisplayName, the rest is
  // blocked
  EXPECT_EQ(presentation->Data(Qt::DisplayRole).toString().toStdString(), item.GetDisplayName());
  EXPECT_FALSE(presentation->Data(Qt::EditRole).isValid());
  EXPECT_FALSE(presentation->SetData(QString("aaa"), Qt::DisplayRole));
  EXPECT_FALSE(presentation->SetData(QString("bbb"), Qt::EditRole));

  // data is the same as before, despite of all attempts to change it
  EXPECT_EQ(presentation->Data(Qt::DisplayRole).toString().toStdString(), item.GetDisplayName());
}

TEST_F(PresentationItemFactoryTest, EditableDisplayNamePresentation)
{
  const std::string expected_name("abc");
  SessionItem item;
  item.SetDisplayName("abc");

  auto presentation = CreateEditableDisplayNamePresentation(&item);

  // item has a display role, which coincide with Instruction's name
  EXPECT_EQ(presentation->Data(Qt::DisplayRole).toString().toStdString(), expected_name);
  EXPECT_EQ(presentation->Data(Qt::EditRole).toString().toStdString(), expected_name);

  // current convention is to disallow setting of DisplayRole
  EXPECT_FALSE(presentation->SetData(QString("abc"), Qt::DisplayRole));
  EXPECT_EQ(presentation->Data(Qt::DisplayRole).toString().toStdString(), expected_name);
  EXPECT_EQ(presentation->Data(Qt::EditRole).toString().toStdString(), expected_name);

  // but it should be possible to set new name via EditRole
  const std::string new_name{"NewName"};
  EXPECT_TRUE(presentation->SetData(QString::fromStdString(new_name), Qt::EditRole));

  // it should change both, edit.display roles
  EXPECT_EQ(presentation->Data(Qt::DisplayRole).toString().toStdString(), new_name);
  EXPECT_EQ(presentation->Data(Qt::EditRole).toString().toStdString(), new_name);

  // the name of instruction should be different
  EXPECT_EQ(item.GetDisplayName(), new_name);
};

TEST_F(PresentationItemFactoryTest, CreateDataPresentation)
{
  const int value{42};

  SessionItem item;
  item.SetData(value, DataRole::kData);

  auto presentation = CreateDataPresentation(&item);

  // item has a display role, which coincide with attribute value
  EXPECT_EQ(presentation->Data(Qt::DisplayRole).toInt(), value);
  EXPECT_EQ(presentation->Data(Qt::EditRole).toInt(), value);

  const int new_value{43};
  EXPECT_FALSE(presentation->SetData(new_value, Qt::DisplayRole));
  EXPECT_TRUE(presentation->SetData(new_value, Qt::EditRole));

  // data is the new one
  EXPECT_EQ(presentation->Data(Qt::DisplayRole).toInt(), new_value);
  EXPECT_EQ(item.Data<int>(), new_value);
};

TEST_F(PresentationItemFactoryTest, CreateDataPresentationWithComposer)
{
  const int value{42};
  SessionModel model;
  auto* item = model.InsertItem<SessionItem>();
  item->SetData(value, DataRole::kData);

  MockModelNotifier notifier;

  ModelComposer composer(&model, &notifier);
  auto set_data = [&composer](auto item, auto data, auto role)
  { return composer.SetData(item, data, role); };
  auto presentation = CreateDataPresentation(item, set_data);

  // The rest of the test is the same as in the test above.
  // Presentation will set data through the composer.

  // item has a display role, which coincide with attribute value
  EXPECT_EQ(presentation->Data(Qt::DisplayRole).toInt(), value);
  EXPECT_EQ(presentation->Data(Qt::EditRole).toInt(), value);

  EXPECT_CALL(notifier, DataChangedNotify(item, DataRole::kData)).Times(1);

  const int new_value{43};
  EXPECT_FALSE(presentation->SetData(new_value, Qt::DisplayRole));
  EXPECT_TRUE(presentation->SetData(new_value, Qt::EditRole));

  // data is the new one
  EXPECT_EQ(presentation->Data(Qt::DisplayRole).toInt(), new_value);
  EXPECT_EQ(item->Data<int>(), new_value);
};
