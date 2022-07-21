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

#include "mock_model.h"
#include "mvvm/model/notifying_model_composer.h"

#include <mvvm/model/model_composer.h>
#include <gtest/gtest.h>
#include <mvvm/interfaces/sessionmodel_interface.h>
#include <mvvm/model/item_utils.h>
#include <mvvm/model/property_item.h>
#include <mvvm/model/sessionitem.h>
#include <mvvm/model/taginfo.h>

using namespace mvvm;
using ::testing::_;

//! Testing ModelComposer class.

class NotifyingModelComposerTests : public ::testing::Test
{
};

TEST_F(NotifyingModelComposerTests, InitialState)
{
  MockModel model;
  NotifyingModelComposer<ModelComposer> composer(model);

  EXPECT_EQ(composer.GetModel(), &model);
}
