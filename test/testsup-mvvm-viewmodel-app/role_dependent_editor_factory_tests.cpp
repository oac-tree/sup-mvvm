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
#include <mvvm/model/item_limits_helper.h>
#include <mvvm/model/property_item.h>
#include <mvvm/standarditems/editor_constants.h>
#include <mvvm/viewmodel/all_items_viewmodel.h>

#include <gtest/gtest.h>

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
   *
   * @param data The data to store as data role.
   * @param editor_type Optional editor name to define for the given cell.
   * @return Index of cell which can be used to access given data.
   */
  QModelIndex AddDataToModel(const variant_t& data, const std::string& editor_type = {})
  {
    // creating item in a model and setting data to it
    auto item = m_model.InsertItem<PropertyItem>();
    item->SetData(data);
    if (!editor_type.empty())
    {
      item->SetEditorType(editor_type);
    }

    // at this point ViewModel was automatically updated, column = 1 is a cell looking to our data
    auto indexes = m_view_model.GetIndexOfSessionItem(item);
    return indexes.back();  // it should be two indices, the second is looking to our data
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
