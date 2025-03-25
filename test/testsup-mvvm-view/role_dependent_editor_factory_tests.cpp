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

#include "mvvm/editors/role_dependent_editor_factory.h"

#include <mvvm/editors/custom_editor_includes.h>
#include <mvvm/model/application_model.h>
#include <mvvm/standarditems/editor_constants.h>
#include <mvvm/viewmodel/all_items_viewmodel.h>

#include <gtest/gtest.h>
#include <testutils/test_viewmodel_helper.h>

#include <QSpinBox>

using namespace mvvm;

/**
 * @brief Tests for editor factories from custom_editor_factories header.
 */
class RoleDependentEditorFactoryTest : public ::testing::Test
{
public:
  RoleDependentEditorFactoryTest() : m_view_model(&m_model) {}

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

TEST_F(RoleDependentEditorFactoryTest, RoleDependentEditorFactory)
{
  const RoleDependentEditorFactory factory;

  // editor for bool types
  auto index = AddDataToModel(variant_t(true), constants::kBoolEditorType);
  EXPECT_TRUE(dynamic_cast<BoolEditor*>(factory.CreateEditor(index).get()));

  // ComboProperty
  index = AddDataToModel(variant_t(ComboProperty()), constants::kComboPropertyEditorType);
  EXPECT_TRUE(dynamic_cast<ComboPropertyEditor*>(factory.CreateEditor(index).get()));

  // ComboProperty
  index = AddDataToModel(variant_t(ComboProperty()), constants::kSelectableComboPropertyEditorType);
  EXPECT_TRUE(dynamic_cast<SelectableComboBoxEditor*>(factory.CreateEditor(index).get()));

  // String as color
  index = AddDataToModel(variant_t("red"), constants::kColorEditorType);
  EXPECT_TRUE(dynamic_cast<ColorEditor*>(factory.CreateEditor(index).get()));

  // double
  index = AddDataToModel(variant_t(42.2), constants::kScientificSpinboxEditorType);
  EXPECT_TRUE(dynamic_cast<ScientificSpinBoxEditor*>(factory.CreateEditor(index).get()));

  // for int we use adapted QSpinBox with limits set
  index = AddDataToModel(variant_t(42), constants::kIntegerEditorType);
  EXPECT_TRUE(dynamic_cast<QSpinBox*>(factory.CreateEditor(index).get()));

  index = AddDataToModel(variant_t(42), constants::kAllIntSpinBoxEditorType);
  EXPECT_TRUE(dynamic_cast<AllIntSpinBoxEditor*>(factory.CreateEditor(index).get()));
}
