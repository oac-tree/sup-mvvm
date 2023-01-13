/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/editors/abstract_editor_factory.h"

#include <gtest/gtest.h>
#include <mvvm/editors/custom_editor.h>

using namespace mvvm;

//! Tests for AbstractEditorFactory.

class AbstractEditorFactoryTests : public ::testing::Test
{
public:
  class TestFactory : public AbstractEditorFactory
  {
  public:
    editor_t CreateEditor(const QModelIndex&) const override { return {}; }
  };

  class TestEditor : public CustomEditor
  {
  protected:
    void UpdateComponents() override {}
  };
};

TEST_F(AbstractEditorFactoryTests, RegisterBuilder)
{
  TestFactory factory;

  auto builder = [](const SessionItem*) -> editor_t { return std::make_unique<TestEditor>(); };
  factory.RegisterBuilder("abc", builder);

  // existing builder. Cast for gtest 1.6 and gcc-7.3.1
  EXPECT_TRUE(static_cast<bool>(factory.FindBuilder("abc")));

  // non existing builder
  EXPECT_FALSE(static_cast<bool>(factory.FindBuilder("edf")));

  EXPECT_THROW(factory.RegisterBuilder("abc", builder), std::runtime_error);
}
