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

#include "mvvm/factories/viewmodel_controller_factory.h"

#include <mvvm/model/application_model.h>
#include <mvvm/model/property_item.h>
#include <mvvm/viewmodel/standard_children_strategies.h>
#include <mvvm/viewmodel/standard_row_strategies.h>
#include <mvvm/viewmodel/viewmodel.h>

#include <gtest/gtest.h>

using namespace mvvm;

class ViewModelControllerFactoryTests : public ::testing::Test
{
public:
  class TestViewModel : public ViewModel
  {
  public:
    void SetControllerIntern(std::unique_ptr<AbstractViewModelController> controller)
    {
      SetController(std::move(controller));
    }
  };

  ApplicationModel m_model;
  TestViewModel m_view_model;
};

TEST_F(ViewModelControllerFactoryTests, CreateController)
{
  auto propertyItem = m_model.InsertItem<PropertyItem>();
  propertyItem->SetData(42.0);

  auto controller =
      factory::CreateController<AllChildrenStrategy, LabelDataRowStrategy>(&m_model, &m_view_model);
  m_view_model.SetControllerIntern(std::move(controller));

  EXPECT_EQ(m_view_model.rowCount(), 1);
  EXPECT_EQ(m_view_model.columnCount(), 2);

  // accessing first child under the root item
  QModelIndex label_index = m_view_model.index(0, 0);
  QModelIndex data_index = m_view_model.index(0, 1);

  EXPECT_EQ(m_view_model.GetSessionItemFromIndex(m_view_model.index(0, 0)), propertyItem);
  EXPECT_EQ(m_view_model.GetSessionItemFromIndex(m_view_model.index(0, 1)), propertyItem);
}
