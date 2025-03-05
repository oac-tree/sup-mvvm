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

#include "mvvm/editors/combo_property_editor.h"

#include <mvvm/viewmodel/variant_converter.h>

#include <gtest/gtest.h>

#include <QComboBox>

using namespace mvvm;

/**
 * @brief Tests for ComboPropertyEditor class.
 */
class ComboPropertyEditorTest : public ::testing::Test
{
};

TEST_F(ComboPropertyEditorTest, SetValueAndToolTip)
{
  ComboPropertyEditor editor;

  const std::vector<std::string> expected_values({"abc", "def"});
  const std::vector<std::string> expected_tooltips({"tooltip1", "tooltip2", "tooltip3"});
  ComboProperty property(expected_values);
  property.SetToolTips(expected_tooltips);

  editor.SetData(GetQtVariant(property));

  auto combo_box = editor.GetComboBox();

  EXPECT_EQ(combo_box->itemText(0).toStdString(), expected_values.at(0));
  EXPECT_EQ(combo_box->itemText(1).toStdString(), expected_values.at(1));
  EXPECT_EQ(combo_box->itemData(0, Qt::ToolTipRole).toString().toStdString(),
            expected_tooltips.at(0));
  EXPECT_EQ(combo_box->itemData(1, Qt::ToolTipRole).toString().toStdString(),
            expected_tooltips.at(1));
  // extra tooltips ignored

  EXPECT_EQ(combo_box->currentIndex(), 0);
}

TEST_F(ComboPropertyEditorTest, SetValueProgrammatically)
{
  ComboPropertyEditor editor;

  const std::vector<std::string> expected_values({"abc", "def"});
  const std::vector<std::string> expected_tooltips({"tooltip1", "tooltip2", "tooltip3"});
  ComboProperty property(expected_values);
  property.SetToolTips(expected_tooltips);

  editor.SetData(GetQtVariant(property));

  auto combo_box = editor.GetComboBox();
  combo_box->setCurrentIndex(1);

  auto new_data = std::get<ComboProperty>(GetStdVariant(editor.GetData()));

  EXPECT_EQ(new_data.GetCurrentIndex(), 1);
}
