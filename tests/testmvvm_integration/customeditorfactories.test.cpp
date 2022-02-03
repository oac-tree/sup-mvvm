/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/editors/customeditorfactories.h"

#include "mvvm/editors/customeditorincludes.h"
#include "mvvm/model/applicationmodel.h"
#include "mvvm/model/propertyitem.h"
#include "mvvm/viewmodel/allitemsviewmodel.h"

#include <gtest/gtest.h>

using namespace mvvm;

//! Tests for AbstractEditorFactory.

class CustomEditorFactoriesTest : public ::testing::Test
{
public:
  CustomEditorFactoriesTest() : m_view_model(&m_model) {}

  //! Returns index pointining to ViewModel cells looking at our data
  QModelIndex GetIndex(const variant_t& data)
  {
    // creating item in a model and setting data to it
    auto item = m_model.InsertItem<PropertyItem>();
    item->SetData(data);

    // at this point ViewModel was automatically updated, column = 1 is a cell looking to our data
    return m_view_model.index(0, 1);
  }

  ApplicationModel m_model;
  AllItemsViewModel m_view_model;
};

TEST_F(CustomEditorFactoriesTest, VariantDependentEditorFactory)
{
  VariantDependentEditorFactory factory;

  // editor for bool types
  auto index = GetIndex(variant_t(true));
  auto editor = factory.CreateEditor(index);
  EXPECT_TRUE(dynamic_cast<BoolEditor*>(editor.get()));
}
