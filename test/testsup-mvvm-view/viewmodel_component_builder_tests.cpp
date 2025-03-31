/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/views/viewmodel_component_builder.h"

#include <mvvm/editors/default_editor_factory.h>
#include <mvvm/providers/i_cell_decorator.h>
#include <mvvm/providers/i_editor_factory.h>
#include <mvvm/views/default_cell_decorator.h>
#include <mvvm/views/viewmodel_delegate.h>

#include <gtest/gtest.h>

using namespace mvvm;

/**
 * @brief Tests of builders in viewmodel_component_builder.h header.
 */
class ViewmodelComponentBuilderTest : public ::testing::Test
{
};

TEST_F(ViewmodelComponentBuilderTest, ViewModelDelegateBuilder)
{
  const std::unique_ptr<ViewModelDelegate> delegate =
      ViewModelDelegateBuilder().Factory<DefaultEditorFactory>().Decorator<DefaultCellDecorator>();
}
