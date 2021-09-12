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
#include "mvvm/viewmodel/viewmodelbase.h"
#include "mvvm/viewmodel/viewmodelutils.h"

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
