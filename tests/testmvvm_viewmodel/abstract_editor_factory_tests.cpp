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

#include "mvvm/editors/abstract_editor_factory.h"

#include <mvvm/editors/custom_editor.h>

#include <gtest/gtest.h>

using namespace mvvm;

//! Tests for AbstractEditorFactory.

class AbstractEditorFactoryTest : public ::testing::Test
{
public:
  class TestFactory : public AbstractEditorFactory
  {
  public:
    std::unique_ptr<CustomEditor> CreateEditor(const QModelIndex&) const override { return {}; }
  };

  class TestEditor : public CustomEditor
  {
  protected:
    void UpdateComponents() override {}
  };
};

TEST_F(AbstractEditorFactoryTest, RegisterBuilder)
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
