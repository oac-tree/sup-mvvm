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

#include "mvvm/model/modelcomposer.h"
#include "mvvm/model/sessionitem.h"
#include "mvvm/model/sessionmodel.h"
#include "mvvm/viewmodel/modeleventnotifier.h"
#include "mvvm/viewmodelbase/viewmodelbase.h"
#include "mvvm/viewmodel/viewmodelutils.h"

#include "test_utils.h"

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
  auto item = m_model.InsertItem<SessionItem>();

  ViewModelController controller(&m_model, &m_viewmodel);
  controller.Init();

  // the model contains only one entry
//  EXPECT_EQ(m_viewmodel.rowCount(), 1);
//  EXPECT_EQ(m_viewmodel.columnCount(), 1);

//  // somewhere beneath there is presentation item looking to our Wait instruction
//  auto item_index = m_viewmodel.index(0, 0);
//  auto view_item = m_viewmodel.itemFromIndex(item_index);
//  EXPECT_EQ(GetInstruction(view_item), wait_ptr);

//  // display role should coincide with the instruction Type
//  EXPECT_EQ(m_viewmodel.data(item_index, Qt::DisplayRole).toString().toStdString(),
//            wait_ptr->GetType());

//  // current behaviour of the presentation item beneath is to forbid editing
//  EXPECT_FALSE(m_viewmodel.data(item_index, Qt::EditRole).isValid());

//  // Finding view from instruction
//  EXPECT_EQ(FindViews<instruction_t>(wait_ptr), std::vector<ModelView::ViewItem*>({view_item}));
}
