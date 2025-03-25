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

#include "mvvm/providers/abstract_editor_factory.h"

#include <mvvm/core/exceptions.h>
#include <mvvm/providers/custom_editor.h>

#include <gtest/gtest.h>

using namespace mvvm;

//! Tests for AbstractEditorFactory.

class AbstractEditorFactoryTest : public ::testing::Test
{
public:
  class TestFactory : public AbstractEditorFactory
  {
  public:
    editor_t CreateEditor(const QModelIndex& index) const override
    {
      (void)index;
      return {};
    }
    editor_t CreateEditor(const SessionItem* item) const override
    {
      (void)item;
      return {};
    }
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

  EXPECT_TRUE(static_cast<bool>(factory.FindBuilder("abc")));

  EXPECT_FALSE(static_cast<bool>(factory.FindBuilder("edf")));

  EXPECT_THROW(factory.RegisterBuilder("abc", builder), RuntimeException);
}
