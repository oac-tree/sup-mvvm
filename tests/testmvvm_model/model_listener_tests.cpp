/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <mvvm/model/application_model.h>
#include <mvvm/signals/model_listener.h>

using namespace mvvm;
using ::testing::_;

//! Testing ModelListener class.

class ModelListenerTests : public ::testing::Test
{
public:
  MOCK_METHOD(void, OnEventV1, (const mvvm::event_variant_t& event));
  MOCK_METHOD(void, OnEventV2, (const mvvm::event_variant_t& event));

//  MOCK_METHOD(void, OnDataChangedEvent, (const mvvm::DataChangedEvent& event));
};

TEST_F(ModelListenerTests, InitialState)
{
  ApplicationModel model;
  ModelListener<ApplicationModel> listener(&model);
  EXPECT_EQ(listener.GetModel(), &model);
}

TEST_F(ModelListenerTests, SingleClientOnEvent)
{
  ApplicationModel model;
  ModelListener<ApplicationModel> listener(&model);
  EXPECT_EQ(listener.GetModel(), &model);
}
