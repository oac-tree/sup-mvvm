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

#include "mvvm/viewmodel/viewmodelcontroller.h"

#include "test_utils.h"

#include "mvvm/model/modelcomposer.h"
#include "mvvm/model/sessionitem.h"
#include "mvvm/model/sessionmodel.h"
#include "mvvm/viewmodel/modeleventnotifier.h"
#include "mvvm/viewmodel/viewmodelutils.h"
#include "mvvm/viewmodelbase/viewmodelbase.h"

#include <gtest/gtest.h>

using namespace ModelView;

//! Tests for ViewModelController class.

class ViewModelControllerTest : public ::testing::Test
{
public:
  ViewModelControllerTest()
      : m_controller(&m_model, &m_viewmodel)
      , m_notifier(&m_controller)
      , m_composer(&m_model, &m_notifier)
  {
  }

  //! Returns underlying SessionItem
  const SessionItem* GetSessionItem(const ViewItem* view_item)
  {
    return Utils::GetContext<SessionItem>(view_item);
  }

  std::vector<ModelView::ViewItem*> FindViews(SessionItem* item)
  {
    return Utils::FindViews<SessionItem>(&m_viewmodel, item);
  }

  SessionModel m_model;
  ViewModelBase m_viewmodel;
  ViewModelController m_controller;
  ModelEventNotifier m_notifier;
  ModelComposer m_composer;
};

//! Empty model.

TEST_F(ViewModelControllerTest, EmptyProcedure)
{
  m_controller.Init();

  EXPECT_EQ(m_viewmodel.rowCount(), 0);
  EXPECT_EQ(m_viewmodel.columnCount(), 0);

  EXPECT_EQ(ModelView::Utils::GetContext<SessionItem>(m_viewmodel.rootItem()),
            m_model.GetRootItem());

  EXPECT_EQ(ModelView::Utils::FindViews(&m_viewmodel, m_model.GetRootItem()),
            std::vector<ModelView::ViewItem*>({m_viewmodel.rootItem()}));
}

//! Attempt to initialize controller with wrong initial parameters.

TEST_F(ViewModelControllerTest, InvalidControllerInitialization)
{
  // should throw if input parameters are invalid
  {
    ViewModelController controller(nullptr, nullptr);
    EXPECT_THROW(controller.Init(), std::runtime_error);
  }

  // should throw if viewmodel is not empty
  {
    std::vector<std::unique_ptr<ModelView::ViewItem>> children;
    children.emplace_back(std::make_unique<ModelView::ViewItem>());
    m_viewmodel.appendRow(m_viewmodel.rootItem(), std::move(children));
    ViewModelController controller(&m_model, &m_viewmodel);
    EXPECT_THROW(controller.Init(), std::runtime_error);
  }
}

//! SessionModel is populated with a single item.
//! Controller is populated after.

TEST_F(ViewModelControllerTest, ModelWithSingleItem)
{
  auto* item = m_model.InsertItem<SessionItem>();
  item->SetData(42.0);

  ViewModelController controller(&m_model, &m_viewmodel);
  controller.Init();

  // the model contains only one entry
  EXPECT_EQ(m_viewmodel.rowCount(), 1);
  EXPECT_EQ(m_viewmodel.columnCount(), 2);

  // default controller constructs a row consisting from item label (display name) and data
  auto label_index = m_viewmodel.index(0, 0);
  auto view_item_label = m_viewmodel.itemFromIndex(label_index);
  EXPECT_EQ(GetSessionItem(view_item_label), item);

  auto data_index = m_viewmodel.index(0, 1);
  auto view_item_data = m_viewmodel.itemFromIndex(data_index);
  EXPECT_EQ(GetSessionItem(view_item_data), item);

  // display roleof first item in a row  should coincide with item's DisplayName
  EXPECT_EQ(m_viewmodel.data(label_index, Qt::DisplayRole).toString().toStdString(),
            item->GetDisplayName());

  // edit role of second item in a row  should coincide with item'sdata
  EXPECT_EQ(m_viewmodel.data(data_index, Qt::EditRole).toDouble(), item->Data<double>());

  // Finding view from instruction
  EXPECT_EQ(FindViews(item), std::vector<ViewItem*>({view_item_label, view_item_data}));
}
