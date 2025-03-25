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

#include "mvvm/editors/variant_dependent_editor_factory.h"

#include <mvvm/editors/custom_editor_includes.h>
#include <mvvm/model/application_model.h>
#include <mvvm/model/item_limits_helper.h>
#include <mvvm/standarditems/editor_constants.h>
#include <mvvm/viewmodel/all_items_viewmodel.h>

#include <gtest/gtest.h>
#include <testutils/test_viewmodel_helper.h>

#include <QSpinBox>

using namespace mvvm;

/**
 * @brief Tests for editor factories from custom_editor_factories header.
 */
class VariantDependentEditorFactoryTest : public ::testing::Test
{
public:
  VariantDependentEditorFactoryTest() : m_view_model(&m_model) {}

  /**
   * @brief Convenience function to add given data to the model as PropertyItem.
   */
  QModelIndex AddDataToModel(const variant_t& data, const std::string& editor_type = {})
  {
    return test::AddDataToModel(data, editor_type, m_view_model);
  }

  ApplicationModel m_model;
  AllItemsViewModel m_view_model;
};

TEST_F(VariantDependentEditorFactoryTest, VariantDependentEditorFactory)
{
  const VariantDependentEditorFactory factory;

  // editor for bool types
  auto index = AddDataToModel(variant_t(true));
  EXPECT_TRUE(dynamic_cast<BoolEditor*>(factory.CreateEditor(index).get()));

  // ComboProperty
  index = AddDataToModel(variant_t(ComboProperty()));
  EXPECT_TRUE(dynamic_cast<ComboPropertyEditor*>(factory.CreateEditor(index).get()));

  // `string` doesn't have custom editor for the moment (handled by default delegate)
  index = AddDataToModel(std::string("abc"));
  EXPECT_FALSE(factory.CreateEditor(index));

  index = AddDataToModel(variant_t(42));
  EXPECT_TRUE(dynamic_cast<QSpinBox*>(factory.CreateEditor(index).get()));

  {  // uint32
    const uint32 num(42);
    index = AddDataToModel(variant_t(num));
    EXPECT_TRUE(dynamic_cast<AllIntSpinBoxEditor*>(factory.CreateEditor(index).get()));
  }

  {  // int64
    const int64 num(42);
    index = AddDataToModel(variant_t(num));
    EXPECT_TRUE(dynamic_cast<AllIntSpinBoxEditor*>(factory.CreateEditor(index).get()));
  }

  {  // uint64
    const uint64 num(42);
    index = AddDataToModel(variant_t(num));
    EXPECT_TRUE(dynamic_cast<AllIntSpinBoxEditor*>(factory.CreateEditor(index).get()));
  }

  {  // float32
    const float32 num(42.1);
    index = AddDataToModel(variant_t(num));
    EXPECT_TRUE(dynamic_cast<FloatSpinBox*>(factory.CreateEditor(index).get()));
  }

  {  // float64
    const float64 num(42.1);
    index = AddDataToModel(variant_t(num));
    EXPECT_TRUE(dynamic_cast<FloatSpinBox*>(factory.CreateEditor(index).get()));
  }
}
