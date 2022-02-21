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

#include "mvvm/factories/viewmodelcontrollerbuilder.h"

#include "mvvm/model/applicationmodel.h"
#include "mvvm/model/propertyitem.h"
#include "mvvm/viewmodel/standardchildrenstrategies.h"
#include "mvvm/viewmodel/standardrowstrategies.h"
#include "mvvm/viewmodel/viewmodel.h"

#include <gtest/gtest.h>

using namespace mvvm;

class ViewModelControllerBuilderTest : public ::testing::Test
{
public:
  class TestViewModel : public ViewModel
  {
  public:
    TestViewModel() {}

    void SetControllerIntern(std::unique_ptr<AbstractViewModelController> controller)
    {
      SetController(std::move(controller));
    }
  };

  ApplicationModel m_model;
  TestViewModel m_view_model;
};

TEST_F(ViewModelControllerBuilderTest, BuilderForAllItemsViewModel)
{
  auto propertyItem = m_model.InsertItem<PropertyItem>();
  propertyItem->SetData(42.0);

  std::unique_ptr<AbstractViewModelController> controller =
      ViewModelControllerBuilder()
          .SetModel(&m_model)
          .SetViewModel(&m_view_model)
          .SetChildrenStrategy(std::make_unique<AllChildrenStrategy>())
          .SetRowStrategy(std::make_unique<LabelDataRowStrategy>());

  m_view_model.SetControllerIntern(std::move(controller));

  EXPECT_EQ(m_view_model.rowCount(), 1);
  EXPECT_EQ(m_view_model.columnCount(), 2);

  // accessing first child under the root item
  QModelIndex label_index = m_view_model.index(0, 0);
  QModelIndex data_index = m_view_model.index(0, 1);

  EXPECT_EQ(m_view_model.GetSessionItemFromIndex(m_view_model.index(0, 0)), propertyItem);
  EXPECT_EQ(m_view_model.GetSessionItemFromIndex(m_view_model.index(0, 1)), propertyItem);
}
